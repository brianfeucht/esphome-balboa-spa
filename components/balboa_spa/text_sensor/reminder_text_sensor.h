#pragma once
#include "esphome/components/text_sensor/text_sensor.h"
#include "../balboaspa.h"

namespace esphome
{
  namespace balboa_spa
  {

    class ReminderTextSensor : public text_sensor::TextSensor
    {
    public:
      void set_parent(BalboaSpa *parent);
      void update(SpaState *spaState);

    private:
      // Store last known value for change detection
      uint8_t last_reminder_ = 0xFF;
    };

  } // namespace balboa_spa
} // namespace esphome
