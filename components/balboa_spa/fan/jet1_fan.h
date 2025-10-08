#pragma once

#include "jet_fan_base.h"

namespace esphome
{
    namespace balboa_spa
    {

        class Jet1Fan : public JetFanBase
        {
        public:
            Jet1Fan() : JetFanBase("BalboaSpa.Jet1Fan", "jet1") {};

        protected:
            double get_jet_state(const SpaState *spaState) override { return spaState->jet1; }
            void toggle_jet() override;
        };

    } // namespace balboa_spa
} // namespace esphome
