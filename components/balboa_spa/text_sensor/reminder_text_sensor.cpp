#include "reminder_text_sensor.h"

namespace esphome
{
    namespace balboa_spa
    {
        static const char *TAG = "balboa_spa.text_sensor.reminder";

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
                std::string reminder_message;
                switch (spaState->reminder)
                {
                    case 0x00:
                        reminder_message = "None";
                        break;
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
                
                ESP_LOGD(TAG, "Reminder update: %s (0x%02X)", 
                         reminder_message.c_str(), spaState->reminder);
                this->publish_state(reminder_message);
                last_reminder_ = spaState->reminder;
            }
        }

    } // namespace balboa_spa
} // namespace esphome
