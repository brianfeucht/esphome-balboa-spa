#pragma once

#include <string>

#include "esphome/components/sensor/sensor.h"
#include "../balboaspa.h"

namespace esphome
{
  namespace balboa_spa
  {

    class BalboaSpaFaultLogSensors : public sensor::Sensor
    {
    public:
      enum class BalboaSpaFaultLogSensorType : uint8_t
      {
        FAULT_CODE = 1,
        FAULT_TOTAL_ENTRIES = 2,
        FAULT_CURRENT_ENTRY = 3,
        FAULT_DAYS_AGO = 4,
      };

    public:
      BalboaSpaFaultLogSensors() {};
      void update(SpaFaultLog *spaFaultLog);

      void set_parent(BalboaSpa *parent);
      void set_sensor_type(BalboaSpaFaultLogSensorType _type) { sensor_type = _type; }

    private:
      BalboaSpaFaultLogSensorType sensor_type;
    };

  } // namespace balboa_spa
} // namespace esphome
