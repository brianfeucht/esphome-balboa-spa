#include "jet4_switch.h"

namespace esphome {
namespace balboa_spa {
void Jet4Switch::update(SpaState* spaState) {
    if(this->state != spaState->jet4)
    {
        this->publish_state(spaState->jet4);
    }
}

void Jet4Switch::set_parent(BalboaSpa *parent) {
    spa = parent;
    parent->register_listener([this](SpaState* spaState){ this->update(spaState); });
}

void Jet4Switch::write_state(bool state)  {
    SpaState* spaState = spa->get_current_state();

    if(spaState->jet4 != state){
        spa->toggle_jet4();
    }
}

}  // namespace balboa_spa
}  // namespace esphome
