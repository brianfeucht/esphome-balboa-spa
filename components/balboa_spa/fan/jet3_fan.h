#pragma once

#include "jet_fan_base.h"

namespace esphome
{
    namespace balboa_spa
    {

        class Jet3Fan : public JetFanBase
        {
        public:
            Jet3Fan() : JetFanBase("BalboaSpa.Jet3Fan", "jet3") {};

        protected:
            double get_jet_state(const SpaState *spaState) override { return spaState->jet3; }
            void toggle_jet() override;
        };

    } // namespace balboa_spa
} // namespace esphome
