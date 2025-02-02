#include "jet1_switch.h"

namespace esphome {
namespace balboa_spa {

void Jet1Switch::update(SpaState* spaState) {
    if(this->state != spaState->jet1)
    {
        this->publish_state(spaState->jet1);
    }
}

void Jet1Switch::set_parent(BalboaSpa *parent) {
    spa = parent;
    parent->register_listener([this](SpaState* spaState){ this->update(spaState); });
}

void Jet1Switch::write_state(bool state) {
    SpaState* spaState = spa->get_current_state();

    if(spaState->jet1 != state){
        spa->toggle_jet1();
    }
}

}  // namespace balboa_spa
}  // namespace esphome