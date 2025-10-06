#pragma once

#include "jet_switch_base.h"

namespace esphome
{
  namespace balboa_spa
  {

    class Jet3Switch : public JetSwitchBase
    {
    public:
      Jet3Switch() : JetSwitchBase("BalboaSpa.Jet3Switch", "jet3") {};

    protected:
      double get_jet_state(const SpaState *spaState) override { return spaState->jet3; }
      void toggle_jet() override;
    };

  } // namespace balboa_spa
} // namespace esphome
