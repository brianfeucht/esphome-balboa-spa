#include "spa_text.h"
#include "esphome/core/log.h"

namespace esphome
{
    namespace balboa_spa
    {
        static const char *TAG = "balboa_spa.text";

        // Helper function to safely parse integer from string without exceptions
        bool safe_parse_int(const std::string &str, int &result)
        {
            if (str.empty())
                return false;
            
            result = 0;
            for (char c : str)
            {
                if (c < '0' || c > '9')
                    return false;
                result = result * 10 + (c - '0');
            }
            return true;
        }

        // Shared time validation function used by all text components
        bool validate_time_format(const std::string &time_str, uint8_t &hour, uint8_t &minute)
        {
            // Check format H:MM or HH:MM (4 or 5 characters)
            if (time_str.length() < 4 || time_str.length() > 5)
            {
                return false;
            }

            // Find the colon position
            size_t colon_pos = time_str.find(':');
            if (colon_pos == std::string::npos || colon_pos == 0 || colon_pos >= time_str.length() - 2)
            {
                return false;
            }

            // Extract hour and minute using safe parsing
            std::string hour_str = time_str.substr(0, colon_pos);
            std::string minute_str = time_str.substr(colon_pos + 1);
            
            // Minute should always be 2 digits
            if (minute_str.length() != 2)
            {
                return false;
            }
            
            int h, m;
            if (!safe_parse_int(hour_str, h) || !safe_parse_int(minute_str, m))
            {
                return false;
            }
            
            if (h >= 0 && h < 24 && m >= 0 && m < 60)
            {
                hour = static_cast<uint8_t>(h);
                minute = static_cast<uint8_t>(m);
                return true;
            }
            
            return false;
        }

        // SpaTimeText implementation
        void SpaTimeText::set_parent(BalboaSpa *parent)
        {
            this->parent_ = parent;
            // Register listener to update from spa state
            parent->register_listener([this](SpaState *state) {
                this->update_from_spa_state(state);
            });
        }

        void SpaTimeText::update_from_spa_state(SpaState *state)
        {
            if (state != nullptr)
            {
                char time_str[6];
                snprintf(time_str, sizeof(time_str), "%02d:%02d", state->hour, state->minutes);
                std::string current_time(time_str);
                
                // Only update if the value has changed to avoid unnecessary updates
                if (this->state != current_time)
                {
                    // Set flag to indicate this update comes from the spa
                    this->updating_from_spa_ = true;
                    this->state = current_time;
                    this->publish_state(current_time);
                    this->updating_from_spa_ = false;
                    ESP_LOGD(TAG, "Spa time updated from tub: %s", current_time.c_str());
                }
            }
        }

        void SpaTimeText::control(const std::string &value)
        {
            // Don't send commands back to spa if this update came from the spa itself
            if (this->updating_from_spa_)
            {
                ESP_LOGD(TAG, "Ignoring spa time control from spa update: %s", value.c_str());
                return;
            }

            uint8_t hour, minute;
            if (validate_time_format(value, hour, minute))
            {
                this->parent_->set_hour(hour);
                this->parent_->set_minute(minute);
                this->state = value;
                this->publish_state(value);
                ESP_LOGI(TAG, "Spa time set to: %s", value.c_str());
            }
            else
            {
                ESP_LOGW(TAG, "Invalid time format: %s. Expected H:MM or HH:MM", value.c_str());
            }
        }

        bool SpaTimeText::validate_time_format(const std::string &time_str, uint8_t &hour, uint8_t &minute)
        {
            return ::esphome::balboa_spa::validate_time_format(time_str, hour, minute);
        }

        // SpaFilter1StartTimeText implementation
        void SpaFilter1StartTimeText::set_parent(BalboaSpa *parent)
        {
            this->parent_ = parent;
            // Register filter listener to update from spa filter settings
            parent->register_filter_listener([this](SpaFilterSettings *settings) {
                this->update_from_filter_settings(settings);
            });
        }

        void SpaFilter1StartTimeText::update_from_filter_settings(SpaFilterSettings *settings)
        {
            if (settings != nullptr)
            {
                char time_str[6];
                snprintf(time_str, sizeof(time_str), "%02d:%02d", settings->filter1_hour, settings->filter1_minute);
                std::string current_time(time_str);
                
                // Only update if the value has changed to avoid unnecessary updates
                if (this->state != current_time)
                {
                    this->state = current_time;
                    this->publish_state(current_time);
                    ESP_LOGD(TAG, "Filter 1 start time updated from tub: %s", current_time.c_str());
                }
            }
        }

        void SpaFilter1StartTimeText::control(const std::string &value)
        {
            uint8_t hour, minute;
            if (validate_time_format(value, hour, minute))
            {
                this->parent_->set_filter1_start_time(hour, minute);
                this->state = value;
                this->publish_state(value);
                ESP_LOGI(TAG, "Filter 1 start time set to: %s", value.c_str());
            }
            else
            {
                ESP_LOGW(TAG, "Invalid time format: %s. Expected H:MM or HH:MM", value.c_str());
            }
        }

        bool SpaFilter1StartTimeText::validate_time_format(const std::string &time_str, uint8_t &hour, uint8_t &minute)
        {
            return ::esphome::balboa_spa::validate_time_format(time_str, hour, minute);
        }

        // SpaFilter1DurationText implementation
        void SpaFilter1DurationText::set_parent(BalboaSpa *parent)
        {
            this->parent_ = parent;
            // Register filter listener to update from spa filter settings
            parent->register_filter_listener([this](SpaFilterSettings *settings) {
                this->update_from_filter_settings(settings);
            });
        }

        void SpaFilter1DurationText::update_from_filter_settings(SpaFilterSettings *settings)
        {
            if (settings != nullptr)
            {
                char time_str[6];
                snprintf(time_str, sizeof(time_str), "%02d:%02d", settings->filter1_duration_hour, settings->filter1_duration_minute);
                std::string current_duration(time_str);
                
                // Only update if the value has changed to avoid unnecessary updates
                if (this->state != current_duration)
                {
                    this->state = current_duration;
                    this->publish_state(current_duration);
                    ESP_LOGD(TAG, "Filter 1 duration updated from tub: %s", current_duration.c_str());
                }
            }
        }

        void SpaFilter1DurationText::control(const std::string &value)
        {
            uint8_t hour, minute;
            if (validate_time_format(value, hour, minute))
            {
                this->parent_->set_filter1_duration(hour, minute);
                this->state = value;
                this->publish_state(value);
                ESP_LOGI(TAG, "Filter 1 duration set to: %s", value.c_str());
            }
            else
            {
                ESP_LOGW(TAG, "Invalid time format: %s. Expected H:MM or HH:MM", value.c_str());
            }
        }

        bool SpaFilter1DurationText::validate_time_format(const std::string &time_str, uint8_t &hour, uint8_t &minute)
        {
            return ::esphome::balboa_spa::validate_time_format(time_str, hour, minute);
        }

        // SpaFilter2StartTimeText implementation
        void SpaFilter2StartTimeText::set_parent(BalboaSpa *parent)
        {
            this->parent_ = parent;
            // Register filter listener to update from spa filter settings
            parent->register_filter_listener([this](SpaFilterSettings *settings) {
                this->update_from_filter_settings(settings);
            });
        }

        void SpaFilter2StartTimeText::update_from_filter_settings(SpaFilterSettings *settings)
        {
            if (settings != nullptr)
            {
                char time_str[6];
                snprintf(time_str, sizeof(time_str), "%02d:%02d", settings->filter2_hour, settings->filter2_minute);
                std::string current_time(time_str);
                
                // Only update if the value has changed to avoid unnecessary updates
                if (this->state != current_time)
                {
                    this->state = current_time;
                    this->publish_state(current_time);
                    ESP_LOGD(TAG, "Filter 2 start time updated from tub: %s", current_time.c_str());
                }
            }
        }

        void SpaFilter2StartTimeText::control(const std::string &value)
        {
            uint8_t hour, minute;
            if (validate_time_format(value, hour, minute))
            {
                this->parent_->set_filter2_start_time(hour, minute);
                this->state = value;
                this->publish_state(value);
                ESP_LOGI(TAG, "Filter 2 start time set to: %s", value.c_str());
            }
            else
            {
                ESP_LOGW(TAG, "Invalid time format: %s. Expected H:MM or HH:MM", value.c_str());
            }
        }

        bool SpaFilter2StartTimeText::validate_time_format(const std::string &time_str, uint8_t &hour, uint8_t &minute)
        {
            return ::esphome::balboa_spa::validate_time_format(time_str, hour, minute);
        }

        // SpaFilter2DurationText implementation
        void SpaFilter2DurationText::set_parent(BalboaSpa *parent)
        {
            this->parent_ = parent;
            // Register filter listener to update from spa filter settings
            parent->register_filter_listener([this](SpaFilterSettings *settings) {
                this->update_from_filter_settings(settings);
            });
        }

        void SpaFilter2DurationText::update_from_filter_settings(SpaFilterSettings *settings)
        {
            if (settings != nullptr)
            {
                char time_str[6];
                snprintf(time_str, sizeof(time_str), "%02d:%02d", settings->filter2_duration_hour, settings->filter2_duration_minute);
                std::string current_duration(time_str);
                
                // Only update if the value has changed to avoid unnecessary updates
                if (this->state != current_duration)
                {
                    this->state = current_duration;
                    this->publish_state(current_duration);
                    ESP_LOGD(TAG, "Filter 2 duration updated from tub: %s", current_duration.c_str());
                }
            }
        }

        void SpaFilter2DurationText::control(const std::string &value)
        {
            uint8_t hour, minute;
            if (validate_time_format(value, hour, minute))
            {
                this->parent_->set_filter2_duration(hour, minute);
                this->state = value;
                this->publish_state(value);
                ESP_LOGI(TAG, "Filter 2 duration set to: %s", value.c_str());
            }
            else
            {
                ESP_LOGW(TAG, "Invalid time format: %s. Expected H:MM or HH:MM", value.c_str());
            }
        }

        bool SpaFilter2DurationText::validate_time_format(const std::string &time_str, uint8_t &hour, uint8_t &minute)
        {
            return ::esphome::balboa_spa::validate_time_format(time_str, hour, minute);
        }

    } // namespace balboa_spa
} // namespace esphome