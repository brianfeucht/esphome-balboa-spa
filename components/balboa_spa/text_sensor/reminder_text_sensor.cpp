#include "reminder_text_sensor.h"

namespace esphome
{
    namespace balboa_spa
    {

        void ReminderTextSensor::set_parent(BalboaSpa *parent)
        {
            parent->register_listener(
                [this](SpaState *spaState)
                {
                    this->update(spaState);
                });
        }

        void ReminderTextSensor::update(SpaState *spaState)
        {
            // Check if the reminder has changed
            if (spaState->reminder != last_reminder_)
            {
                // NOTE: This list of reminder codes is incomplete. If you encounter an unknown
                // code, please open a GitHub issue with the code value and the reminder message
                // displayed on your spa control panel so we can expand this mapping.
                std::string reminder_message;
                switch (spaState->reminder)
                {
                    case 0x00:
                        reminder_message = "None";
                        break;
                    case 0x03:
                    case 0x04:
                        reminder_message = "Clean Filter";
                        break;
                    case 0x09:
                        reminder_message = "Check Sanitizer";
                        break;
                    case 0x0A:
                        reminder_message = "Check pH";
                        break;
                    case 0x1E:
                        reminder_message = "Fault";
                        break;
                    default:
                        // Format unknown reminder code in hex
                        char hex_str[8];
                        snprintf(hex_str, sizeof(hex_str), "0x%02X", spaState->reminder);
                        reminder_message = std::string("Unknown (") + hex_str + ")";
                        break;
                }
                
                this->publish_state(reminder_message);
                last_reminder_ = spaState->reminder;
            }
        }

    } // namespace balboa_spa
} // namespace esphome
