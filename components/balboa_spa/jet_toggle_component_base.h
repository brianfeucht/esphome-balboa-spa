#pragma once

#include "esphome/core/component.h"
#include "balboaspa.h"

namespace esphome
{
    namespace balboa_spa
    {
        /**
         * @brief Base class for components that need toggle logic with retry capability
         *
         * This class provides shared toggle logic for both switches and fans that control
         * jets. It includes MAX_TOGGLE_ATTEMPTS functionality to handle cases where the
         * spa doesn't allow state changes (e.g., during heating or filter cycles).
         */
        class JetToggleComponentBase
        {
        public:
            JetToggleComponentBase(const char *tag, const char *jet_name)
                : tag(tag), jet_name(jet_name) {};

            void set_parent(BalboaSpa *parent);
            void set_max_toggle_attempts(uint8_t value) { this->max_toggle_attempts = value; }
            void set_discard_updates(uint8_t value) { this->discard_updates_config = value; }

        protected:
            /**
             * @brief Get the current jet state from spa state
             * @param spaState Current spa state
             * @return Current jet state (0=OFF, 1=LOW, 2=HIGH)
             */
            virtual double get_jet_state(const SpaState *spaState) = 0;

            /**
             * @brief Trigger a toggle command for this jet
             */
            virtual void toggle_jet() = 0;

            /**
             * @brief Update logic called when spa state changes
             * @param spaState Current spa state
             * @param desired_state Desired jet state (0=OFF, 1=LOW, 2=HIGH)
             * @param current_state Reference to current component state for comparison
             * @param publish_callback Callback to publish new state
             */
            void update_toggle_state(
                const SpaState *spaState,
                int desired_state,
                int &current_state,
                std::function<void(int)> publish_callback);

            /**
             * @brief Request a state change with toggle logic
             * @param target_state Target state (0=OFF, 1=LOW, 2=HIGH)
             * @param current_jet_state Current jet state from spa
             */
            void request_state_change(int target_state, double current_jet_state);

            BalboaSpa *spa = nullptr;
            const char *tag;
            const char *jet_name;

        private:
            ToggleStateMaybe desired_state = ToggleStateMaybe::DONT_KNOW;
            uint8_t toggle_attempts = 0;
            uint8_t max_toggle_attempts = 5;
            uint8_t discard_updates = 0;
            uint8_t discard_updates_config = 5;
        };

    } // namespace balboa_spa
} // namespace esphome
