#pragma once

#include "esphome/core/component.h"
#include "esphome/components/fan/fan.h"
#include "../balboaspa.h"

namespace esphome {
namespace balboa_spa {

class Jet1Fan : public fan::Fan {
 public:
  Jet1Fan() {};
  void update(SpaState* spaState);
  void set_parent(BalboaSpa *parent);

  protected:
    void control(const fan::FanCall &call) override;
    fan::FanTraits get_traits() override;
    
  private:
    BalboaSpa *spa;
    
    // Convert 3-state spa state (0=off, 1=low, 2=high) to speed
    int spa_state_to_speed(uint8_t spa_state);
    // Convert speed to spa state (0=off, 1=low, 2=high)
    uint8_t speed_to_spa_state(int speed);
};

}  // namespace balboa_spa
}  // namespace esphome