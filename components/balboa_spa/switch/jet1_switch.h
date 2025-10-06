#pragma once

#include "jet_switch_base.h"

namespace esphome
{
  namespace balboa_spa
  {

    class Jet1Switch : public JetSwitchBase
    {
    public:
      Jet1Switch() : JetSwitchBase("BalboaSpa.Jet1Switch", "jet1") {};

    protected:
      double get_jet_state(const SpaState *spaState) override { return spaState->jet1; }
      void toggle_jet() override;
    };

  } // namespace balboa_spa
} // namespace esphome
