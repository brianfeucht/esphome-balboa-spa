#include "jet_switch_base.h"

namespace esphome
{
    namespace balboa_spa
    {
        void JetSwitchBase::update(const SpaState *spaState)
        {
            if (this->discard_updates > 0)
            {
                this->discard_updates--;
                return;
            }

            // Convert multi-state jet to boolean (0=OFF, 1,2=ON)
            double jet_raw_state = this->get_jet_state(spaState);
            bool jet_is_on = jet_raw_state > 0;

            if (this->state != jet_is_on && this->setState == ToggleStateMaybe::DONT_KNOW)
            {
                this->publish_state(jet_is_on);
                ESP_LOGD(tag_, "Spa/%s/switch: state updated to %s (raw: %.0f)", jet_name_, jet_is_on ? STRON : STROFF, jet_raw_state);
            }
            else if (this->setState == ToggleStateMaybe::ON && !jet_is_on)
            {
                // Want ON but currently OFF - toggle to get to LOW speed
                this->toggle_jet();
                ESP_LOGD(tag_, "Spa/%s/switch: want ON, currently OFF (%.0f), toggling", jet_name_, jet_raw_state);
            }
            else if (this->setState == ToggleStateMaybe::OFF && jet_is_on)
            {
                // Want OFF but currently ON (LOW or HIGH) - keep toggling until OFF
                this->toggle_jet();
                ESP_LOGD(tag_, "Spa/%s/switch: want OFF, currently ON (%.0f), toggling", jet_name_, jet_raw_state);
            }
            else if (this->setState != ToggleStateMaybe::DONT_KNOW)
            {
                // We've reached the desired state
                this->setState = ToggleStateMaybe::DONT_KNOW;
                ESP_LOGD(tag_, "Spa/%s/switch: reached desired state (%.0f), setState is now DONT_KNOW", jet_name_, jet_raw_state);
            }
        }

        void JetSwitchBase::set_parent(BalboaSpa *parent)
        {
            spa = parent;
            parent->register_listener([this](const SpaState *spaState)
                                      { this->update(spaState); });
        }

        void JetSwitchBase::write_state(bool state)
        {
            SpaState *spaState = spa->get_current_state();
            double jet_raw_state = this->get_jet_state(spaState);
            bool jet_is_on = jet_raw_state > 0;

            if (jet_is_on != state)
            {
                this->setState = state ? ToggleStateMaybe::ON : ToggleStateMaybe::OFF;
                this->toggle_jet();
                ESP_LOGD(tag_, "Spa/%s/switch: toggling, wants %s, discard_updates set to %d", jet_name_, TOGGLE_STATE_MAYBE_STRINGS[this->setState], this->discard_updates_config_);
                this->discard_updates = this->discard_updates_config_;
            }
        }

    } // namespace balboa_spa
} // namespace esphome
