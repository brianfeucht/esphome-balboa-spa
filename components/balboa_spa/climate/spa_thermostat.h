#pragma once

#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"
#include "../balboaspa.h"

namespace esphome {
namespace balboa_spa {

class BalboaSpaThermostat : public climate::Climate, public Component {
 public:
  BalboaSpaThermostat() {
    spa = nullptr;
    lastUpdate = 0;
  };

  void update(SpaState* spaState);
  void set_parent(BalboaSpa *parent);

 protected:
  void control(const climate::ClimateCall &call) override;
  climate::ClimateTraits traits() override;

 private:
  BalboaSpa *spa;
  uint32_t lastUpdate;
};

}  // namespace balboa_spa
}  // namespace esphome
