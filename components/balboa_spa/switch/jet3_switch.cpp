#include "jet3_switch.h"

namespace esphome {
namespace balboa_spa {

void Jet3Switch::update(const SpaState* spaState) {
    if (this->discard_updates > 0) {
        this->discard_updates--;
        return;
    }
    if(this->state != spaState->jet3 && this->setState == ToggleStateMaybe::DONT_KNOW) {
        this->publish_state(spaState->jet3);
        ESP_LOGD("jet3_switch", "Jet3 switch state updated to %s", spaState->jet3 ? STRON : STROFF);
    } else if (this->setState == ToggleStateMaybe::ON && !spaState->jet3) {
        this->toggle_jet3();
        ESP_LOGD("jet3_switch", "Jet3 state changed %s setState is ON, toggling jet3", state ? STRON : STROFF);
    } else if (this->setState == ToggleStateMaybe::OFF && spaState->jet3) {
        this->toggle_jet3();
        ESP_LOGD("jet3_switch", "Jet3 state changed %s setState is OFF, toggling jet3", state ? STRON : STROFF);
    } else if (this->setState != ToggleStateMaybe::DONT_KNOW) {
        this->setState = ToggleStateMaybe::DONT_KNOW;
        ESP_LOGD("jet3_switch", "write_state successful, setState is now DONT_KNOW");
    }
}

void Jet3Switch::toggle_jet3() {
    spa->toggle_jet3();
    this->discard_updates = this->discard_updates_config_;
    ESP_LOGD("jet3_switch", "Jet3 wants %s, discard_updates set to %d", TOGGLE_STATE_MAYBE_STRINGS[this->setState], this->discard_updates);
}

void Jet3Switch::set_parent(BalboaSpa *parent) {
    spa = parent;
    parent->register_listener([this](const SpaState* spaState){ this->update(spaState); });
}

void Jet3Switch::write_state(bool state) {
    SpaState* spaState = spa->get_current_state();

    if(spaState->jet3 != state){
        this->setState = state ? ToggleStateMaybe::ON : ToggleStateMaybe::OFF;
        this->toggle_jet3();
    }
}

}  // namespace balboa_spa
}  // namespace esphome