#include "jet_fan_base.h"

namespace esphome
{
    namespace balboa_spa
    {
        fan::FanTraits JetFanBase::get_traits()
        {
            return fan::FanTraits(false, true, false, 2);
        }

        void JetFanBase::update(const SpaState *spaState)
        {
            // Fan states: 0=OFF, 1=LOW, 2=HIGH
            double jet_raw_state = this->get_jet_state(spaState);
            int jet_state = static_cast<int>(jet_raw_state);

            // Determine desired state from fan control
            int desired_state = 0;
            if (this->state)
            {
                // Fan is ON - determine speed (1=LOW, 2=HIGH)
                desired_state = (this->speed >= 2) ? 2 : 1;
            }

            // Use base class update logic
            this->update_toggle_state(
                spaState,
                desired_state,
                current_fan_state,
                [this](int new_state)
                {
                    if (new_state == 0)
                    {
                        // OFF
                        if (this->state != false)
                        {
                            this->state = false;
                            this->speed = 0;
                            this->publish_state();
                        }
                    }
                    else if (new_state == 1)
                    {
                        // LOW
                        if (this->state != true || this->speed != 1)
                        {
                            this->state = true;
                            this->speed = 1;
                            this->publish_state();
                        }
                    }
                    else if (new_state == 2)
                    {
                        // HIGH
                        if (this->state != true || this->speed != 2)
                        {
                            this->state = true;
                            this->speed = 2;
                            this->publish_state();
                        }
                    }
                });
        }

        void JetFanBase::set_parent(BalboaSpa *parent)
        {
            JetToggleComponentBase::set_parent(parent);
            parent->register_listener([this](const SpaState *spaState)
                                      { this->update(spaState); });
        }

        void JetFanBase::control(const fan::FanCall &call)
        {
            SpaState *spaState = spa->get_current_state();
            double jet_raw_state = this->get_jet_state(spaState);

            // Handle state change
            if (call.get_state().has_value())
            {
                bool new_state = *call.get_state();
                if (new_state)
                {
                    // Turning ON - check if speed is specified
                    int target_speed = 1; // Default to LOW
                    if (call.get_speed().has_value())
                    {
                        int speed_val = *call.get_speed();
                        target_speed = (speed_val >= 2) ? 2 : 1;
                    }
                    else if (this->speed > 0)
                    {
                        // Use current speed if already set
                        target_speed = this->speed;
                    }

                    ESP_LOGD(tag, "Spa/%s/fan: turning ON at speed %d", jet_name, target_speed);
                    this->state = true;
                    this->speed = target_speed;
                    this->request_state_change(target_speed, jet_raw_state);
                }
                else
                {
                    // Turning OFF
                    ESP_LOGD(tag, "Spa/%s/fan: turning OFF", jet_name);
                    this->state = false;
                    this->speed = 0;
                    this->request_state_change(0, jet_raw_state);
                }
            }
            // Handle speed change while already ON
            else if (call.get_speed().has_value() && this->state)
            {
                int new_speed = *call.get_speed();
                int target_speed = (new_speed >= 2) ? 2 : 1;

                ESP_LOGD(tag, "Spa/%s/fan: changing speed to %d", jet_name, target_speed);
                this->speed = target_speed;
                this->request_state_change(target_speed, jet_raw_state);
            }

            this->publish_state();
        }

    } // namespace balboa_spa
} // namespace esphome
