#include "jet_switch_base.h"

namespace esphome
{
    namespace balboa_spa
    {
        void JetSwitchBase::update(const SpaState *spaState)
        {
            // Use the base class toggle logic, but convert to boolean for switch
            // Switch: OFF (0) or ON (1 or 2)
            double jet_raw_state = this->get_jet_state(spaState);
            int jet_state = static_cast<int>(jet_raw_state);
            bool switch_state = (jet_state > 0);

            // Determine what we want: if switch is ON, we want any non-zero state (prefer 1=LOW)
            // If switch is OFF, we want state 0
            int desired_state = this->state ? 1 : 0;

            // Use base class update logic
            this->update_toggle_state(
                spaState,
                desired_state,
                current_switch_state_,
                [this](int new_state)
                {
                    bool new_switch_state = (new_state > 0);
                    if (this->state != new_switch_state)
                    {
                        this->publish_state(new_switch_state);
                    }
                });
        }

        void JetSwitchBase::set_parent(BalboaSpa *parent)
        {
            JetToggleComponentBase::set_parent(parent);
            parent->register_listener([this](const SpaState *spaState)
                                      { this->update(spaState); });
        }

        void JetSwitchBase::write_state(bool state)
        {
            SpaState *spaState = spa->get_current_state();
            double jet_raw_state = this->get_jet_state(spaState);

            // For switch: OFF = 0, ON = 1 (LOW speed)
            int target_state = state ? 1 : 0;

            this->request_state_change(target_state, jet_raw_state);
        }

    } // namespace balboa_spa
} // namespace esphome
