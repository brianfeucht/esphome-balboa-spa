#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "../jet_toggle_component_base.h"

namespace esphome
{
    namespace balboa_spa
    {

        class JetSwitchBase : public switch_::Switch, public JetToggleComponentBase
        {
        public:
            JetSwitchBase(const char *tag, const char *jet_name)
                : JetToggleComponentBase(tag, jet_name) {};

            void update(const SpaState *spaState);
            void set_parent(BalboaSpa *parent);
            void set_discard_updates(uint8_t value)
            {
                // Support both legacy name and new separate config
                this->set_discard_updates_count(value);
            }
            void set_max_toggle_attempts(uint8_t value) { JetToggleComponentBase::set_max_toggle_attempts(value); }
            void set_discard_updates_count(uint8_t value) { JetToggleComponentBase::set_discard_updates(value); }

        protected:
            void write_state(bool state) override;
            virtual double get_jet_state(const SpaState *spaState) = 0;
            virtual void toggle_jet() = 0;

        private:
            int current_switch_state_ = 0; // 0=OFF, >0=ON for switch
        };

    } // namespace balboa_spa
} // namespace esphome
