#pragma once

#include "jet_switch_base.h"

namespace esphome
{
  namespace balboa_spa
  {

    class Jet2Switch : public JetSwitchBase
    {
    public:
      Jet2Switch() : JetSwitchBase("BalboaSpa.Jet2Switch", "jet2") {};

    protected:
      double get_jet_state(const SpaState *spaState) override { return spaState->jet2; }
      void toggle_jet() override;
    };

  } // namespace balboa_spa
} // namespace esphome
