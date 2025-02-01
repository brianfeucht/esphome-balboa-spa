#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "../balboaspa.h"

namespace esphome {
namespace balboa_spa {

class BalboaSpaSensors : public PollingComponent, public sensor::Sensor {
public:
  enum class BalboaSpaSensorType{
    BLOWER,
    HIGHRANGE,
    CIRCULATION,
  };

public:
  BalboaSpaSensors() : PollingComponent(1000) {}
  void setup() override;
  void dump_config() override;
  void update() override;

  void set_sensor_id(uint8_t sensor_id) { this->sensor_id_ = sensor_id; }

  void set_parent(BalboaSpa *parent) { spa = parent; }
  void set_sensor_type(BalboaSpaSensorType _type) { sensor_type = _type;  }

  private:
    BalboaSpa *spa;
    uint8_t sensor_id_{0};
    BalboaSpaSensorType sensor_type;
};

}  // namespace balboa_spa
}  // namespace esphome
