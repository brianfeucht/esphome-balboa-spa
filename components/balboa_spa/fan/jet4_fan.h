#pragma once

#include "jet_fan_base.h"

namespace esphome
{
    namespace balboa_spa
    {

        class Jet4Fan : public JetFanBase
        {
        public:
            Jet4Fan() : JetFanBase("BalboaSpa.Jet4Fan", "jet4") {};

        protected:
            double get_jet_state(const SpaState *spaState) override { return spaState->jet4; }
            void toggle_jet() override;
        };

    } // namespace balboa_spa
} // namespace esphome
