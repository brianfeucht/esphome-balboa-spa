#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "../balboaspa.h"

namespace esphome {
namespace balboa_spa {

class BalboaSpaSensors : public PollingComponent, public sensor::Sensor {
public:
  enum class BalboaSpaSensorType : uint8_t{
    BLOWER = 1,
    HIGHRANGE = 2,
    CIRCULATION = 3,
  };

public:
  BalboaSpaSensors() : PollingComponent(1000) {}
  void update() override;

  void set_parent(BalboaSpa *parent) { spa = parent; }
  void set_sensor_type(BalboaSpaSensorType _type) { sensor_type = _type;  }

  private:
    BalboaSpa *spa;
    BalboaSpaSensorType sensor_type;
};

}  // namespace balboa_spa
}  // namespace esphome
