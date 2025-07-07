#include "jet1_switch.h"

namespace esphome {
namespace balboa_spa {

void Jet1Switch::update(SpaState* spaState) {
    // Convert multi-state (0=off, 1=low, 2=high) to boolean (0=off, >0=on)
    bool jet_on = spaState->jet1 > 0;
    if(this->state != jet_on)
    {
        this->publish_state(jet_on);
    }
}

void Jet1Switch::set_parent(BalboaSpa *parent) {
    spa = parent;
    parent->register_listener([this](SpaState* spaState){ this->update(spaState); });
}

void Jet1Switch::write_state(bool state) {
    SpaState* spaState = spa->get_current_state();
    bool current_on = spaState->jet1 > 0;

    if(current_on != state){
        spa->toggle_jet1();
    }
}

}  // namespace balboa_spa
}  // namespace esphome