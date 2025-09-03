#include "spa_text.h"
#include "esphome/core/log.h"
#include <sstream>
#include <regex>

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

        // SpaFilter1ConfigText implementation
        void SpaFilter1ConfigText::set_parent(BalboaSpa *parent)
        {
            this->parent_ = parent;
        }

        void SpaFilter1ConfigText::control(const std::string &value)
        {
            uint8_t start_hour, start_minute, duration_hour, duration_minute;
            if (validate_filter_config(value, start_hour, start_minute, duration_hour, duration_minute))
            {
                this->parent_->set_filter1_config(start_hour, start_minute, duration_hour, duration_minute);
                this->state = value;
                this->publish_state(value);
                ESP_LOGI(TAG, "Filter 1 config set to: %s", value.c_str());
            }
            else
            {
                ESP_LOGW(TAG, "Invalid filter config format: %s. Expected {\"start\":\"HH:MM\",\"duration\":\"HH:MM\"}", value.c_str());
            }
        }

        bool SpaFilter1ConfigText::validate_filter_config(const std::string &config_str, uint8_t &start_hour, uint8_t &start_minute, uint8_t &duration_hour, uint8_t &duration_minute)
        {
            // Simple regex pattern matching for JSON format
            std::regex pattern("\\{\\s*\"start\"\\s*:\\s*\"(\\d{1,2}):(\\d{1,2})\"\\s*,\\s*\"duration\"\\s*:\\s*\"(\\d{1,2}):(\\d{1,2})\"\\s*\\}");
            std::smatch matches;
            
            if (std::regex_match(config_str, matches, pattern))
            {
                int sh, sm, dh, dm;
                if (!safe_parse_int(matches[1], sh) || !safe_parse_int(matches[2], sm) ||
                    !safe_parse_int(matches[3], dh) || !safe_parse_int(matches[4], dm))
                {
                    return false;
                }
                
                if (sh >= 0 && sh < 24 && sm >= 0 && sm < 60 && 
                    dh >= 0 && dh < 24 && dm >= 0 && dm < 60)
                {
                    start_hour = static_cast<uint8_t>(sh);
                    start_minute = static_cast<uint8_t>(sm);
                    duration_hour = static_cast<uint8_t>(dh);
                    duration_minute = static_cast<uint8_t>(dm);
                    return true;
                }
            }
            
            return false;
        }

        // SpaFilter2ConfigText implementation
        void SpaFilter2ConfigText::set_parent(BalboaSpa *parent)
        {
            this->parent_ = parent;
        }

        void SpaFilter2ConfigText::control(const std::string &value)
        {
            if (value == "disabled")
            {
                this->parent_->disable_filter2();
                this->state = value;
                this->publish_state(value);
                ESP_LOGI(TAG, "Filter 2 disabled");
                return;
            }

            uint8_t start_hour, start_minute, duration_hour, duration_minute;
            if (validate_filter_config(value, start_hour, start_minute, duration_hour, duration_minute))
            {
                this->parent_->set_filter2_config(start_hour, start_minute, duration_hour, duration_minute);
                this->state = value;
                this->publish_state(value);
                ESP_LOGI(TAG, "Filter 2 config set to: %s", value.c_str());
            }
            else
            {
                ESP_LOGW(TAG, "Invalid filter config format: %s. Expected {\"start\":\"HH:MM\",\"duration\":\"HH:MM\"} or \"disabled\"", value.c_str());
            }
        }

        bool SpaFilter2ConfigText::validate_filter_config(const std::string &config_str, uint8_t &start_hour, uint8_t &start_minute, uint8_t &duration_hour, uint8_t &duration_minute)
        {
            // Same validation as Filter1
            std::regex pattern("\\{\\s*\"start\"\\s*:\\s*\"(\\d{1,2}):(\\d{1,2})\"\\s*,\\s*\"duration\"\\s*:\\s*\"(\\d{1,2}):(\\d{1,2})\"\\s*\\}");
            std::smatch matches;
            
            if (std::regex_match(config_str, matches, pattern))
            {
                int sh, sm, dh, dm;
                if (!safe_parse_int(matches[1], sh) || !safe_parse_int(matches[2], sm) ||
                    !safe_parse_int(matches[3], dh) || !safe_parse_int(matches[4], dm))
                {
                    return false;
                }
                
                if (sh >= 0 && sh < 24 && sm >= 0 && sm < 60 && 
                    dh >= 0 && dh < 24 && dm >= 0 && dm < 60)
                {
                    start_hour = static_cast<uint8_t>(sh);
                    start_minute = static_cast<uint8_t>(sm);
                    duration_hour = static_cast<uint8_t>(dh);
                    duration_minute = static_cast<uint8_t>(dm);
                    return true;
                }
            }
            
            return false;
        }

    } // namespace balboa_spa
} // namespace esphome