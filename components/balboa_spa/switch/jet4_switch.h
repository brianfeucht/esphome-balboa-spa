#pragma once

#include "jet_switch_base.h"

namespace esphome
{
  namespace balboa_spa
  {

    class Jet4Switch : public JetSwitchBase
    {
    public:
      Jet4Switch() : JetSwitchBase("BalboaSpa.Jet4Switch", "jet4") {};

    protected:
      double get_jet_state(const SpaState *spaState) override { return spaState->jet4; }
      void toggle_jet() override;
    };

  } // namespace balboa_spa
} // namespace esphome
