#include "jet2_switch.h"

namespace esphome
{
    namespace balboa_spa
    {

        void Jet2Switch::update(const SpaState *spaState)
        {
            if (this->discard_updates > 0)
            {
                this->discard_updates--;
                return;
            }
            if (this->state != spaState->jet2 && this->setState == ToggleStateMaybe::DONT_KNOW)
            {
                this->publish_state(spaState->jet2);
                ESP_LOGD("jet2_switch", "Jet2 switch state updated to %s", spaState->jet2 ? STRON : STROFF);
            }
            else if (this->setState == ToggleStateMaybe::ON && !spaState->jet2)
            {
                this->toggle_jet2();
                ESP_LOGD("jet2_switch", "Jet2 state changed %s setState is ON, toggling jet2", state ? STRON : STROFF);
            }
            else if (this->setState == ToggleStateMaybe::OFF && spaState->jet2)
            {
                this->toggle_jet2();
                ESP_LOGD("jet2_switch", "Jet2 state changed %s setState is OFF, toggling jet2", state ? STRON : STROFF);
            }
            else if (this->setState != ToggleStateMaybe::DONT_KNOW)
            {
                this->setState = ToggleStateMaybe::DONT_KNOW;
                ESP_LOGD("jet2_switch", "write_state successful, setState is now DONT_KNOW");
            }
        }

        void Jet2Switch::toggle_jet2()
        {
            spa->toggle_jet2();
            this->discard_updates = this->discard_updates_config_;
            ESP_LOGD("jet2_switch", "Jet2 wants %s, discard_updates set to %d", TOGGLE_STATE_MAYBE_STRINGS[this->setState], this->discard_updates);
        }

        void Jet2Switch::set_parent(BalboaSpa *parent)
        {
            spa = parent;
            parent->register_listener([this](const SpaState *spaState)
                                      { this->update(spaState); });
        }

        void Jet2Switch::write_state(bool state)
        {
            SpaState *spaState = spa->get_current_state();

            if (spaState->jet2 != state)
            {
                this->setState = state ? ToggleStateMaybe::ON : ToggleStateMaybe::OFF;
                this->toggle_jet2();
            }
        }

    } // namespace balboa_spa
} // namespace esphome