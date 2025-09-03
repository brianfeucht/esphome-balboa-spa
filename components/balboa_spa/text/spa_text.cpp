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
            // Check format HH:MM
            if (time_str.length() != 5 || time_str[2] != ':')
            {
                return false;
            }

            // Extract hour and minute using safe parsing
            std::string hour_str = time_str.substr(0, 2);
            std::string minute_str = time_str.substr(3, 2);
            
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
        }

        void SpaTimeText::control(const std::string &value)
        {
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
                ESP_LOGW(TAG, "Invalid time format: %s. Expected HH:MM", value.c_str());
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
                ESP_LOGW(TAG, "Invalid time format: %s. Expected HH:MM", value.c_str());
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
                ESP_LOGW(TAG, "Invalid time format: %s. Expected HH:MM", value.c_str());
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
                ESP_LOGW(TAG, "Invalid time format: %s. Expected HH:MM", value.c_str());
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
                ESP_LOGW(TAG, "Invalid time format: %s. Expected HH:MM", value.c_str());
            }
        }

        bool SpaFilter2DurationText::validate_time_format(const std::string &time_str, uint8_t &hour, uint8_t &minute)
        {
            return ::esphome::balboa_spa::validate_time_format(time_str, hour, minute);
        }

    } // namespace balboa_spa
} // namespace esphome