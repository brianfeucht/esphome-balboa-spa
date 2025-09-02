#pragma once

#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"
#include "../balboaspa.h"

namespace esphome
{
  namespace balboa_spa
  {

    class BalboaSpaThermostat : public climate::Climate, public Component
    {
    public:
      BalboaSpaThermostat()
      {
        spa = nullptr;
        last_update_time = 0;
      };

      void update(SpaState *spaState);
      void set_parent(BalboaSpa *parent);

    protected:
      void control(const climate::ClimateCall &call) override;
      climate::ClimateTraits traits() override;

    private:
      BalboaSpa *spa;
      uint32_t last_update_time;
    };

  } // namespace balboa_spa
} // namespace esphome
