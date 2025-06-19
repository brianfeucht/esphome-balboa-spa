#include "jet3_switch.h"

namespace esphome {
namespace balboa_spa {
void Jet3Switch::update(SpaState* spaState) {
    if(this->state != spaState->jet3)
    {
        this->publish_state(spaState->jet3);
    }
}

void Jet3Switch::set_parent(BalboaSpa *parent) {
    spa = parent;
    parent->register_listener([this](SpaState* spaState){ this->update(spaState); });
}

void Jet3Switch::write_state(bool state)  { 
    SpaState* spaState = spa->get_current_state();

    if(spaState->jet3 != state){
        spa->toggle_jet3();
    }
}

}  // namespace balboa_spa
}  // namespace esphome
