#pragma once

#include "jet_fan_base.h"

namespace esphome
{
    namespace balboa_spa
    {

        class Jet2Fan : public JetFanBase
        {
        public:
            Jet2Fan() : JetFanBase("BalboaSpa.Jet2Fan", "jet2") {};

        protected:
            double get_jet_state(const SpaState *spaState) override { return spaState->jet2; }
            void toggle_jet() override;
        };

    } // namespace balboa_spa
} // namespace esphome
