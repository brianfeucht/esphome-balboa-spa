#include "lights_switch.h"

namespace esphome {
namespace balboa_spa {
void LightsSwitch::update(SpaState* spaState) {
    if(this->state != spaState->light)
    {
        this->publish_state(spaState->light);
    }
}

void LightsSwitch::set_parent(BalboaSpa *parent) {
    spa = parent;
    parent->register_listener([this](SpaState* spaState){ this->update(spaState); });
}

void LightsSwitch::write_state(bool state) {
    SpaState* spaState = spa->get_current_state();

    if(spaState->light != state){
        spa->toggle_light();
    }
}

}  // namespace balboa_spa
}  // namespace esphome
