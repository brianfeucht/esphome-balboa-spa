#pragma once

#include "esphome/core/component.h"
#include "esphome/components/fan/fan.h"
#include "../balboaspa.h"

namespace esphome {
namespace balboa_spa {

class Jet4Fan : public fan::Fan {
 public:
  Jet4Fan() {};
  void update(SpaState* spaState);
  void set_parent(BalboaSpa *parent);

  protected:
    void control(const fan::FanCall &call) override;
    fan::FanTraits get_traits() override;
    
  private:
    BalboaSpa *spa;
    
    int spa_state_to_speed(uint8_t spa_state);
    uint8_t speed_to_spa_state(int speed);
};

}  // namespace balboa_spa
}  // namespace esphome