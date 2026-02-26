#pragma once

#include "esphome/components/water_heater/water_heater.h"
#include "../balboaspa.h"

namespace esphome
{
  namespace balboa_spa
  {

    // Modes:
    //   OFF         → rest_mode=1 (sleep/rest, energy-saving standby)
    //   ECO         → rest_mode=0, highrange=0 (ready, standard temp range)
    //   PERFORMANCE → rest_mode=0, highrange=1 (ready, high temp range)

    class BalboaSpaWaterHeater : public water_heater::WaterHeater
    {
    public:
      BalboaSpaWaterHeater()
      {
        spa = nullptr;
        last_update_time = 0;
      };

      void setup() override;
      void update(SpaState *spaState);
      void set_parent(BalboaSpa *parent);

      water_heater::WaterHeaterCallInternal make_call() override
      {
        return water_heater::WaterHeaterCallInternal(this);
      }

    protected:
      void control(const water_heater::WaterHeaterCall &call) override;
      water_heater::WaterHeaterTraits traits() override;

    private:
      BalboaSpa *spa;
      uint32_t last_update_time;
    };

  } // namespace balboa_spa
} // namespace esphome
