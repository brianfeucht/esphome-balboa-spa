#include "jet_toggle_component_base.h"

namespace esphome
{
    namespace balboa_spa
    {
        void JetToggleComponentBase::set_parent(BalboaSpa *parent)
        {
            spa = parent;
        }

        void JetToggleComponentBase::update_toggle_state(
            const SpaState *spaState,
            int desired_state,
            int &current_state,
            std::function<void(int)> publish_callback)
        {
            // Skip updates immediately after sending a command
            if (this->discard_updates > 0)
            {
                this->discard_updates--;
                ESP_LOGD(tag, "Spa/%s: discarding update (%d remaining)", jet_name, this->discard_updates);
                return;
            }

            double jet_raw_state = this->get_jet_state(spaState);
            int jet_state = static_cast<int>(jet_raw_state);

            // If we're not trying to change state, just update to match spa
            if (this->desired_state == ToggleStateMaybe::DONT_KNOW)
            {
                if (jet_state != current_state)
                {
                    current_state = jet_state;
                    publish_callback(jet_state);
                    ESP_LOGD(tag, "Spa/%s: state updated to %d", jet_name, jet_state);
                }
                return;
            }

            // We're trying to reach a desired state
            int target_state = 0;
            if (this->desired_state == ToggleStateMaybe::OFF)
            {
                target_state = 0;
            }
            else if (this->desired_state == ToggleStateMaybe::ON)
            {
                target_state = 1;
            }
            else if (this->desired_state == ToggleStateMaybe::HIGH_SPEED)
            {
                target_state = 2;
            }

            // Check if we've reached the target state
            if (jet_state == target_state)
            {
                this->desired_state = ToggleStateMaybe::DONT_KNOW;
                this->toggle_attempts = 0;
                current_state = jet_state;
                publish_callback(jet_state);
                ESP_LOGD(tag, "Spa/%s: reached target state %d", jet_name, target_state);
                return;
            }

            // Still need to toggle to reach target
            if (this->toggle_attempts < this->max_toggle_attempts)
            {
                this->toggle_attempts++;
                this->discard_updates = this->discard_updates_config;
                this->toggle_jet();
                ESP_LOGD(tag, "Spa/%s: toggling (attempt %d/%d) current=%d, target=%d, discarding next %d updates",
                         jet_name, this->toggle_attempts, this->max_toggle_attempts,
                         jet_state, target_state, this->discard_updates);
            }
            else
            {
                // Max attempts reached, give up and sync with spa state
                ESP_LOGW(tag, "Spa/%s: max toggle attempts (%d) reached, syncing with spa state %d",
                         jet_name, this->max_toggle_attempts, jet_state);
                this->desired_state = ToggleStateMaybe::DONT_KNOW;
                this->toggle_attempts = 0;
                current_state = jet_state;
                publish_callback(jet_state);
            }
        }

        void JetToggleComponentBase::request_state_change(int target_state, double current_jet_state)
        {
            int current_state = static_cast<int>(current_jet_state);

            if (current_state == target_state)
            {
                ESP_LOGD(tag, "Spa/%s: already at target state %d", jet_name, target_state);
                return;
            }

            // Set desired state
            if (target_state == 0)
            {
                this->desired_state = ToggleStateMaybe::OFF;
            }
            else if (target_state == 1)
            {
                this->desired_state = ToggleStateMaybe::ON;
            }
            else if (target_state == 2)
            {
                this->desired_state = ToggleStateMaybe::HIGH_SPEED;
            }

            this->toggle_attempts = 0;
            this->discard_updates = this->discard_updates_config;
            this->toggle_jet();
            ESP_LOGD(tag, "Spa/%s: requesting state change from %d to %d, discarding next %d updates",
                     jet_name, current_state, target_state, this->discard_updates);
        }

    } // namespace balboa_spa
} // namespace esphome
