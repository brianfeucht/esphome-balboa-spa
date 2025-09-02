#pragma once

#include <string>

#include "esphome/components/binary_sensor/binary_sensor.h"
#include "../balboaspa.h"

namespace esphome
{
  namespace balboa_spa
  {

    class BalboaSpaBinarySensors : public binary_sensor::BinarySensor
    {
    public:
      enum class BalboaSpaBinarySensorType : uint8_t
      {
        UNKNOWN = 0,
        BLOWER,
        HIGHRANGE,
        CIRCULATION,
        RESTMODE,
        HEATSTATE,
        CONNECTED
      };

    public:
      BalboaSpaBinarySensors();
      void update(SpaState *spaState);

      void set_parent(BalboaSpa *parent);
      void set_sensor_type(const BalboaSpaBinarySensorType _type) { sensor_type = _type; }

    private:
      BalboaSpaBinarySensorType sensor_type;
      BalboaSpa *spa;
      uint32_t last_update_time;
    };

  } // namespace balboa_spa
} // namespace esphome
