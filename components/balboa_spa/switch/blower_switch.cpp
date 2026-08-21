#include "blower_switch.h"

namespace esphome
{
    namespace balboa_spa
    {

        void BlowerSwitch::update(const SpaState *spaState)
        {
            if (this->discard_updates > 0)
            {
                this->discard_updates--;
                return;
            }
            if (this->state != spaState->blower && this->setState == ToggleStateMaybe::DONT_KNOW)
            {
                this->publish_state(spaState->blower);
                ESP_LOGD("blower_swtich", "Blower switch state updated to %s", spaState->blower ? STRON : STROFF);
            }
            else if ((this->setState == ToggleStateMaybe::ON && !spaState->blower) ||
                     (this->setState == ToggleStateMaybe::OFF && spaState->blower))
            {
                // Bounded retry: without a limit this loops forever whenever the
                // spa refuses the command, or when the mainboard has no blower
                // at all and never reports the requested state.
                if (this->toggle_attempts_ < this->max_toggle_attempts_)
                {
                    this->toggle_attempts_++;
                    this->toggle_blower();
                    ESP_LOGD("blower_switch", "Toggling blower (attempt %d/%d), setState is %s",
                             this->toggle_attempts_, this->max_toggle_attempts_,
                             TOGGLE_STATE_MAYBE_STRINGS[this->setState]);
                }
                else
                {
                    ESP_LOGW("blower_switch", "Failed to reach target state after %d attempts - "
                                              "spa may be in filter cycle, or this spa has no blower",
                             this->max_toggle_attempts_);
                    this->setState = ToggleStateMaybe::DONT_KNOW;
                    this->toggle_attempts_ = 0;
                    this->publish_state(spaState->blower);
                }
            }
            else if (this->setState != ToggleStateMaybe::DONT_KNOW)
            {
                this->setState = ToggleStateMaybe::DONT_KNOW;
                this->toggle_attempts_ = 0;
                ESP_LOGD("blower_switch", "write_state successful, setState is now DONT_KNOW");
            }
        }

        void BlowerSwitch::toggle_blower()
        {
            spa->toggle_blower();
            this->discard_updates = this->discard_updates_config_;
            ESP_LOGD("blower_swtich", "Blower wants %s, discard_updates set to %d", TOGGLE_STATE_MAYBE_STRINGS[this->setState], this->discard_updates);
        }

        void BlowerSwitch::set_parent(BalboaSpa *parent)
        {
            spa = parent;
            parent->register_listener([this](const SpaState *spaState)
                                      { this->update(spaState); });
        }

        void BlowerSwitch::write_state(bool state)
        {
            SpaState *spaState = spa->get_current_state();

            if (spaState->blower != state)
            {
                this->setState = state ? ToggleStateMaybe::ON : ToggleStateMaybe::OFF;
                this->toggle_attempts_ = 0;
                this->toggle_blower();
            }
        }

    } // namespace balboa_spa
} // namespace esphome
