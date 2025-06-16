#include "blower_switch.h"

namespace esphome {
namespace balboa_spa {
void BlowerSwitch::update(SpaState* spaState) {
    if(this->state != spaState->blower)
    {
        this->publish_state(spaState->blower);
    }
}

void BlowerSwitch::set_parent(BalboaSpa *parent) {
    spa = parent;
    parent->register_listener([this](SpaState* spaState){ this->update(spaState); });
}

void BlowerSwitch::write_state(bool state) {
    SpaState* spaState = spa->get_current_state();

    if(spaState->blower != state){
        spa->toggle_blower();
    }
}

}  // namespace balboa_spa
}  // namespace esphome
