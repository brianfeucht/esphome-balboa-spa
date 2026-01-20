#pragma once
#include "esphome/components/text_sensor/text_sensor.h"
#include "../balboaspa.h"

namespace esphome
{
  namespace balboa_spa
  {

    class FaultMessageTextSensor : public text_sensor::TextSensor
    {
    public:
      void set_parent(BalboaSpa *parent);
      void update(SpaFaultLog *spaFaultLog);

    private:
      // Store last known value for change detection
      std::string last_message_ = "";
    };

  } // namespace balboa_spa
} // namespace esphome
