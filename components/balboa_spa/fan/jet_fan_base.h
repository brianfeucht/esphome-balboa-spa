#pragma once

#include "esphome/core/component.h"
#include "esphome/components/fan/fan.h"
#include "../jet_toggle_component_base.h"

namespace esphome
{
    namespace balboa_spa
    {
        /**
         * @brief Base class for jet fan components
         *
         * Provides fan interface for multi-speed jets (OFF/LOW/HIGH).
         * Uses JetToggleComponentBase for reliable state transitions with retry logic.
         */
        class JetFanBase : public fan::Fan, public JetToggleComponentBase
        {
        public:
            JetFanBase(const char *tag, const char *jet_name)
                : JetToggleComponentBase(tag, jet_name) {};

            void update(const SpaState *spaState);
            void set_parent(BalboaSpa *parent);
            void set_max_toggle_attempts(uint8_t value) { JetToggleComponentBase::set_max_toggle_attempts(value); }
            void set_discard_updates(uint8_t value) { JetToggleComponentBase::set_discard_updates(value); }

            fan::FanTraits get_traits() override;

        protected:
            void control(const fan::FanCall &call) override;
            virtual double get_jet_state(const SpaState *spaState) = 0;
            virtual void toggle_jet() = 0;

        private:
            int current_fan_state = 0; // 0=OFF, 1=LOW, 2=HIGH
        };

    } // namespace balboa_spa
} // namespace esphome
