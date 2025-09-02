#include "spa_filter_config_text_sensor.h"

namespace esphome
{
    namespace balboa_spa
    {

        void SpaFilter1ConfigTextSensor::set_parent(BalboaSpa *parent)
        {
            this->parent_ = parent;
            parent->register_listener([this](SpaState *spaState)
                                      { 
        auto filterSettings = this->parent_->get_current_filter_settings();
        this->update(filterSettings); });
        }

        void SpaFilter1ConfigTextSensor::update(SpaFilterSettings *filterSettings)
        {
            // Check if any values have changed
            if (filterSettings->filter1_hour != last_filter1_hour_ ||
                filterSettings->filter1_minute != last_filter1_minute_ ||
                filterSettings->filter1_duration_hour != last_filter1_duration_hour_ ||
                filterSettings->filter1_duration_minute != last_filter1_duration_minute_)
            {

                // Format: {"start":"HH:MM","duration":"HH:MM"}
                char buf[50];
                snprintf(buf, sizeof(buf), "{\"start\":\"%02u:%02u\",\"duration\":\"%02u:%02u\"}",
                         filterSettings->filter1_hour,
                         filterSettings->filter1_minute,
                         filterSettings->filter1_duration_hour,
                         filterSettings->filter1_duration_minute);
                this->publish_state(buf);

                // Update last known values
                last_filter1_hour_ = filterSettings->filter1_hour;
                last_filter1_minute_ = filterSettings->filter1_minute;
                last_filter1_duration_hour_ = filterSettings->filter1_duration_hour;
                last_filter1_duration_minute_ = filterSettings->filter1_duration_minute;
            }
        }

        void SpaFilter2ConfigTextSensor::set_parent(BalboaSpa *parent)
        {
            this->parent_ = parent;
            parent->register_listener(
                [this](SpaState *spaState)
                {
                    auto filterSettings = this->parent_->get_current_filter_settings();
                    this->update(filterSettings);
                });
        }

        void SpaFilter2ConfigTextSensor::update(SpaFilterSettings *filterSettings)
        {
            // Check if any values have changed
            if (filterSettings->filter2_enable != last_filter2_enable_ ||
                filterSettings->filter2_hour != last_filter2_hour_ ||
                filterSettings->filter2_minute != last_filter2_minute_ ||
                filterSettings->filter2_duration_hour != last_filter2_duration_hour_ ||
                filterSettings->filter2_duration_minute != last_filter2_duration_minute_)
            {

                if (filterSettings->filter2_enable)
                {
                    // Format: {"start":"HH:MM","duration":"HH:MM"}
                    char buf[50];
                    snprintf(buf, sizeof(buf), "{\"start\":\"%02u:%02u\",\"duration\":\"%02u:%02u\"}",
                             filterSettings->filter2_hour,
                             filterSettings->filter2_minute,
                             filterSettings->filter2_duration_hour,
                             filterSettings->filter2_duration_minute);
                    this->publish_state(buf);
                }
                else
                {
                    this->publish_state("disabled");
                }

                // Update last known values
                last_filter2_enable_ = filterSettings->filter2_enable;
                last_filter2_hour_ = filterSettings->filter2_hour;
                last_filter2_minute_ = filterSettings->filter2_minute;
                last_filter2_duration_hour_ = filterSettings->filter2_duration_hour;
                last_filter2_duration_minute_ = filterSettings->filter2_duration_minute;
            }
        }

    } // namespace balboa_spa
} // namespace esphome
