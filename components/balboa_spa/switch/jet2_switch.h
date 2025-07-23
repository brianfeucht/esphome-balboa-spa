#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "../balboaspa.h"

namespace esphome {
namespace balboa_spa {

class Jet2Switch : public switch_::Switch {
  public:
    Jet2Switch() {};
    void update(const SpaState* spaState);
    void set_parent(BalboaSpa *parent);
    void set_discard_updates(uint8_t value) { this->discard_updates_config_ = value; }

  protected:
    void write_state(bool state) override;

  private:
    void toggle_jet2();
    BalboaSpa *spa = nullptr;
    ToggleStateMaybe setState = ToggleStateMaybe::DONT_KNOW;
    uint8_t discard_updates = 0;
    uint8_t discard_updates_config_ = 10;
};

}  // namespace balboa_spa
}  // namespace esphome