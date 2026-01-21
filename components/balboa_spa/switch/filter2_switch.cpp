#include "filter2_switch.h"

namespace esphome
{
    namespace balboa_spa
    {
        static const char *TAG = "Filter2Switch";

        void Filter2Switch::update(SpaFilterSettings *filterSettings)
        {
            if (this->state != filterSettings->filter2_enable)
            {
                this->publish_state(filterSettings->filter2_enable);
            }
        }

        void Filter2Switch::set_parent(BalboaSpa *parent)
        {
            this->parent_ = parent;
            // Subscribe to filter settings updates to sync switch state
            parent->register_filter_listener([this](SpaFilterSettings *filterSettings) {
                this->update(filterSettings);
            });
        }

        void Filter2Switch::write_state(bool state)
        {
            SpaFilterSettings *settings = this->parent_->get_current_filter_settings();
            
            if (state)
            {
                // User wants to turn ON filter 2
                // Check if we have a valid configuration to enable
                if (settings == nullptr || (settings->filter2_hour == 0 && settings->filter2_minute == 0 && 
                                            settings->filter2_duration_hour == 0 && settings->filter2_duration_minute == 0))
                {
                    ESP_LOGE(TAG, "Cannot enable Filter 2: No configuration set. Use filter2_start_time and filter2_duration text components to configure first.");
                    this->publish_state(false);
                    return;
                }

                // Enable filter 2 with the current settings
                this->parent_->set_filter2_config(
                    settings->filter2_hour,
                    settings->filter2_minute,
                    settings->filter2_duration_hour,
                    settings->filter2_duration_minute
                );
            }
            else
            {
                // User wants to turn OFF filter 2
                this->parent_->disable_filter2();
            }
        }

    } // namespace balboa_spa
} // namespace esphome
