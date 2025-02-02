#include "jet2_switch.h"

namespace esphome {
namespace balboa_spa {
void Jet2Switch::update(SpaState* spaState) {
    if(this->state != spaState->jet2)
    {
        this->publish_state(spaState->jet2);
    }
}

void Jet2Switch::set_parent(BalboaSpa *parent) {
    spa = parent;
    parent->register_listener([this](SpaState* spaState){ this->update(spaState); });
}

void Jet2Switch::write_state(bool state)  { 
    SpaState* spaState = spa->get_current_state();

    if(spaState->jet2 != state){
        spa->toggle_jet2();
    }
}

}  // namespace balboa_spa
}  // namespace esphome
