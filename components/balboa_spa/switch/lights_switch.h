#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "../balboaspa.h"

namespace esphome {
namespace balboa_spa {

class LightsSwitch : public PollingComponent, public switch_::Switch {
 public:
  LightsSwitch();
  void update() override;
  void set_parent(BalboaSpa *parent);

  protected:
    void write_state(bool state) override;
    
  private:
    BalboaSpa *spa;
};

}  // namespace balboa_spa
}  // namespace esphome
