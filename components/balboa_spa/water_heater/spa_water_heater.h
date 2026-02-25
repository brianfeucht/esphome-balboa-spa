#pragma once

#include "esphome/core/component.h"
#include "esphome/components/water_heater/water_heater.h"
#include "../balboaspa.h"

namespace esphome
{
  namespace balboa_spa
  {

    class BalboaSpaWaterHeater : public water_heater::WaterHeater, public Component
    {
    public:
      BalboaSpaWaterHeater()
      {
        spa = nullptr;
        last_update_time = 0;
      };

      void update(SpaState *spaState);
      void set_parent(BalboaSpa *parent);

    protected:
      void control(const water_heater::WaterHeaterCall &call) override;
      water_heater::WaterHeaterTraits traits() override;

    private:
      BalboaSpa *spa;
      uint32_t last_update_time;
    };

  } // namespace balboa_spa
} // namespace esphome
