#include "jet1_fan.h"

static const char *TAG = "balboa_spa.jet1_fan";

namespace esphome {
namespace balboa_spa {

void Jet1Fan::update(SpaState* spaState) {
    // Convert spa state (0=off, 1=low, 2=high) to fan state
    bool new_state = spaState->jet1 > 0;
    int new_speed = spa_state_to_speed(spaState->jet1);
    
    if (this->state != new_state || this->speed != new_speed) {
        this->state = new_state;
        this->speed = new_speed;
        this->publish_state();
    }
}

void Jet1Fan::set_parent(BalboaSpa *parent) {
    spa = parent;
    parent->register_listener([this](SpaState* spaState){ this->update(spaState); });
}

void Jet1Fan::control(const fan::FanCall &call) {
    if (call.get_state().has_value()) {
        if (*call.get_state()) {
            // Turning on - set to low speed if no speed specified
            uint8_t target_state = 1; // Low speed
            if (call.get_speed().has_value()) {
                target_state = speed_to_spa_state(*call.get_speed());
            }
            spa->set_jet1_state(target_state);
        } else {
            // Turning off
            spa->set_jet1_state(0);
        }
    } else if (call.get_speed().has_value()) {
        // Speed change only
        uint8_t target_state = speed_to_spa_state(*call.get_speed());
        spa->set_jet1_state(target_state);
    }
}

fan::FanTraits Jet1Fan::get_traits() {
    auto traits = fan::FanTraits();
    traits.set_supports_speed(true);
    traits.set_speed_count(3); // Off, Low, High (speed count doesn't include off)
    traits.set_supports_direction(false);
    traits.set_supports_oscillation(false);
    return traits;
}

int Jet1Fan::spa_state_to_speed(uint8_t spa_state) {
    // Convert spa state (0=off, 1=low, 2=high) to speed (1-3, where 1=low, 3=high)
    switch (spa_state) {
        case 0: return 0;  // Off
        case 1: return 1;  // Low (33%)
        case 2: return 3;  // High (100%)
        default: return 0;
    }
}

uint8_t Jet1Fan::speed_to_spa_state(int speed) {
    // Convert speed (0-3) to spa state (0=off, 1=low, 2=high)
    // ESPHome fan speeds: 0=off, 1=33%, 2=66%, 3=100%
    // Spa states: 0=off, 1=low, 2=high
    if (speed <= 0) return 0; // Off
    if (speed == 1) return 1; // Low (33%)
    return 2; // High (66% and 100% both map to high)
}

}  // namespace balboa_spa
}  // namespace esphome