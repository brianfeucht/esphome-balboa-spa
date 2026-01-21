#include "filter2_switch.h"

namespace esphome
{
    namespace balboa_spa
    {
        static const char *TAG = "balboa_spa.switch";

        void Filter2Switch::update(SpaFilterSettings *filterSettings)
        {
            if (this->state != filterSettings->filter2_enable)
            {
                this->publish_state(filterSettings->filter2_enable);
            }
        }

        void Filter2Switch::set_parent(BalboaSpa *parent)
        {
            spa = parent;
            // Subscribe to filter settings updates to sync switch state
            parent->register_filter_listener([this](SpaFilterSettings *filterSettings) {
                this->update(filterSettings);
            });
        }

        void Filter2Switch::write_state(bool state)
        {
            SpaFilterSettings *settings = spa->get_current_filter_settings();
            
            if (state)
            {
                // User wants to turn ON filter 2
                // Check if we have a valid duration configured (start time can be 00:00 which is valid)
                if (settings == nullptr || (settings->filter2_duration_hour == 0 && settings->filter2_duration_minute == 0))
                {
                    ESP_LOGE(TAG, "Cannot enable Filter 2: Invalid or no duration configured. Use filter2_duration text component to set a valid duration first.");
                    this->publish_state(false);
                    return;
                }

                // Enable filter 2 with the current settings
                spa->set_filter2_config(
                    settings->filter2_hour,
                    settings->filter2_minute,
                    settings->filter2_duration_hour,
                    settings->filter2_duration_minute
                );
                ESP_LOGI(TAG, "Filter 2 enabled with start time %02d:%02d and duration %02d:%02d", 
                         settings->filter2_hour, settings->filter2_minute,
                         settings->filter2_duration_hour, settings->filter2_duration_minute);
                // Request fresh filter settings to ensure switch state syncs promptly
                spa->request_filter_settings_update();
            }
            else
            {
                // User wants to turn OFF filter 2
                spa->disable_filter2();
                ESP_LOGI(TAG, "Filter 2 disabled");
                // Request fresh filter settings to ensure switch state syncs promptly
                spa->request_filter_settings_update();
            }
        }

    } // namespace balboa_spa
} // namespace esphome
