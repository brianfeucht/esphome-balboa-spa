#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "../balboaspa.h"

namespace esphome
{
  namespace balboa_spa
  {

    class HighrangeSwitch : public switch_::Switch
    {
    public:
      HighrangeSwitch() {};
      void update(const SpaState *spaState);
      void set_parent(BalboaSpa *parent);

    protected:
      void write_state(bool state) override;

    private:
      BalboaSpa *spa = nullptr;
    };

  } // namespace balboa_spa
} // namespace esphome
