#include "light2_switch.h"

namespace esphome {
namespace balboa_spa {
void Light2Switch::update(SpaState* spaState) {
    if(this->state != spaState->light2)
    {
        this->publish_state(spaState->light2);
    }
}

void Light2Switch::set_parent(BalboaSpa *parent) {
    spa = parent;
    parent->register_listener([this](SpaState* spaState){ this->update(spaState); });
}

void Light2Switch::write_state(bool state) {
    SpaState* spaState = spa->get_current_state();

    if(spaState->light2 != state){
        spa->toggle_light2();
    }
}

}  // namespace balboa_spa
}  // namespace esphome
