#pragma once

#include <string>

#include "esphome/components/sensor/sensor.h"
#include "../balboaspa.h"

namespace esphome {
namespace balboa_spa {

class BalboaSpaSensors : public sensor::Sensor {
public:
  enum class BalboaSpaSensorType : uint8_t{
    UNKNOWN = 0,
    BLOWER,
    HIGHRANGE,
    CIRCULATION,
    RESTMODE,
    HEATSTATE,
    CONNECTED,
    SENSOR_A,
    SENSOR_B
  };

public:
  BalboaSpaSensors();
  void update(SpaState* spaState);

  void set_parent(BalboaSpa *parent);
  void set_sensor_type(BalboaSpaSensorType _type) { sensor_type = _type;  }

  private:
    BalboaSpaSensorType sensor_type;
    BalboaSpa *spa;
    uint32_t lastUpdate;
};

}  // namespace balboa_spa
}  // namespace esphome
