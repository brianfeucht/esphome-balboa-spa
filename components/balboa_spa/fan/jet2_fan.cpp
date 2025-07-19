#include "jet2_fan.h"

static const char *TAG = "balboa_spa.jet2_fan";

namespace esphome {
namespace balboa_spa {

void Jet2Fan::update(SpaState* spaState) {
    bool new_state = spaState->jet2 > 0;
    int new_speed = spa_state_to_speed(spaState->jet2);
    
    if (this->state != new_state || this->speed != new_speed) {
        this->state = new_state;
        this->speed = new_speed;
        this->publish_state();
    }
}

void Jet2Fan::set_parent(BalboaSpa *parent) {
    spa = parent;
    parent->register_listener([this](SpaState* spaState){ this->update(spaState); });
}

void Jet2Fan::control(const fan::FanCall &call) {
    if (call.get_state().has_value()) {
        if (*call.get_state()) {
            uint8_t target_state = 1; // Low speed
            if (call.get_speed().has_value()) {
                target_state = speed_to_spa_state(*call.get_speed());
            }
            spa->set_jet2_state(target_state);
        } else {
            spa->set_jet2_state(0);
        }
    } else if (call.get_speed().has_value()) {
        uint8_t target_state = speed_to_spa_state(*call.get_speed());
        spa->set_jet2_state(target_state);
    }
}

fan::FanTraits Jet2Fan::get_traits() {
    auto traits = fan::FanTraits();
    traits.set_supports_speed(true);
    traits.set_speed_count(3);
    traits.set_supports_direction(false);
    traits.set_supports_oscillation(false);
    return traits;
}

int Jet2Fan::spa_state_to_speed(uint8_t spa_state) {
    switch (spa_state) {
        case 0: return 0;  // Off
        case 1: return 1;  // Low
        case 2: return 3;  // High
        default: return 0;
    }
}

uint8_t Jet2Fan::speed_to_spa_state(int speed) {
    // Convert speed (0-3) to spa state (0=off, 1=low, 2=high)
    // ESPHome fan speeds: 0=off, 1=33%, 2=66%, 3=100%
    // Spa states: 0=off, 1=low, 2=high
    if (speed <= 0) return 0; // Off
    if (speed == 1) return 1; // Low (33%)
    return 2; // High (66% and 100% both map to high)
}

}  // namespace balboa_spa
}  // namespace esphome