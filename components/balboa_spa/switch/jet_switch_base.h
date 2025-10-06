#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "../balboaspa.h"

namespace esphome
{
    namespace balboa_spa
    {

        class JetSwitchBase : public switch_::Switch
        {
        public:
            JetSwitchBase(const char *tag, const char *jet_name) : tag_(tag), jet_name_(jet_name) {};
            void update(const SpaState *spaState);
            void set_parent(BalboaSpa *parent);
            void set_discard_updates(uint8_t value) { this->discard_updates_config_ = value; }

        protected:
            void write_state(bool state) override;
            virtual double get_jet_state(const SpaState *spaState) = 0;
            virtual void toggle_jet() = 0;
            BalboaSpa *spa = nullptr;

        private:
            const char *tag_;
            const char *jet_name_;
            ToggleStateMaybe setState = ToggleStateMaybe::DONT_KNOW;
            uint8_t discard_updates = 0;
            uint8_t discard_updates_config_ = 10;
        };

    } // namespace balboa_spa
} // namespace esphome
