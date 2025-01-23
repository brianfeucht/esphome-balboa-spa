#include "jet2_switch.h"

namespace esphome {
namespace balboa_spa {

Jet2Switch::Jet2Switch() : PollingComponent(1000) {}

void Jet2Switch::update() {
    SpaState spaState = spa->get_current_state();

    if(this->state != spaState.jet2)
    {
        this->publish_state(spaState.jet2);
    }
}

void Jet2Switch::set_parent(BalboaSpa *parent) { spa = parent; }

void Jet2Switch::write_state(bool state)  {
    if(this->state != state){
        spa->toggle_jet2();
    }
}

}  // namespace balboa_spa
}  // namespace esphome
