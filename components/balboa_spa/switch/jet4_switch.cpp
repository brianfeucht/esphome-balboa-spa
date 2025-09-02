#include "jet4_switch.h"

namespace esphome
{
    namespace balboa_spa
    {

        void Jet4Switch::update(const SpaState *spaState)
        {
            if (this->discard_updates > 0)
            {
                this->discard_updates--;
                return;
            }
            if (this->state != spaState->jet4 && this->setState == ToggleStateMaybe::DONT_KNOW)
            {
                this->publish_state(spaState->jet4);
                ESP_LOGD("jet4_switch", "Jet4 switch state updated to %s", spaState->jet4 ? STRON : STROFF);
            }
            else if (this->setState == ToggleStateMaybe::ON && !spaState->jet4)
            {
                this->toggle_jet4();
                ESP_LOGD("jet4_switch", "Jet4 state changed %s setState is ON, toggling jet4", state ? STRON : STROFF);
            }
            else if (this->setState == ToggleStateMaybe::OFF && spaState->jet4)
            {
                this->toggle_jet4();
                ESP_LOGD("jet4_switch", "Jet4 state changed %s setState is OFF, toggling jet4", state ? STRON : STROFF);
            }
            else if (this->setState != ToggleStateMaybe::DONT_KNOW)
            {
                this->setState = ToggleStateMaybe::DONT_KNOW;
                ESP_LOGD("jet4_switch", "write_state successful, setState is now DONT_KNOW");
            }
        }

        void Jet4Switch::toggle_jet4()
        {
            spa->toggle_jet4();
            this->discard_updates = this->discard_updates_config_;
            ESP_LOGD("jet4_switch", "Jet4 wants %s, discard_updates set to %d", TOGGLE_STATE_MAYBE_STRINGS[this->setState], this->discard_updates);
        }

        void Jet4Switch::set_parent(BalboaSpa *parent)
        {
            spa = parent;
            parent->register_listener([this](const SpaState *spaState)
                                      { this->update(spaState); });
        }

        void Jet4Switch::write_state(bool state)
        {
            SpaState *spaState = spa->get_current_state();

            if (spaState->jet4 != state)
            {
                this->setState = state ? ToggleStateMaybe::ON : ToggleStateMaybe::OFF;
                this->toggle_jet4();
            }
        }

    } // namespace balboa_spa
} // namespace esphome