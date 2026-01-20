#include "esphome/core/log.h"
#include "binary_sensors.h"

namespace esphome
{
    namespace balboa_spa
    {

        static const char *TAG = "BalboaSpa.binary_sensors";
        
        // Constant for time calculations
        static constexpr int MINUTES_PER_DAY = 24 * 60; // 1440 minutes in a day

        /**
         * Helper function to check if a filter cycle is currently running
         * @param current_hour Current spa hour (0-23)
         * @param current_minute Current spa minute (0-59)
         * @param start_hour Filter start hour (0-23)
         * @param start_minute Filter start minute (0-59)
         * @param duration_hour Filter duration hours (0-23)
         * @param duration_minute Filter duration minutes (0-59)
         * @return true if filter should be running, false otherwise
         */
        static bool is_filter_running(uint8_t current_hour, uint8_t current_minute,
                                       uint8_t start_hour, uint8_t start_minute,
                                       uint8_t duration_hour, uint8_t duration_minute)
        {
            // Convert times to total minutes since midnight for easier comparison
            const int current_minutes = current_hour * 60 + current_minute;
            const int start_minutes = start_hour * 60 + start_minute;
            const int duration_total_minutes = duration_hour * 60 + duration_minute;
            const int end_minutes = start_minutes + duration_total_minutes;

            // Handle wrap-around midnight case
            if (end_minutes >= MINUTES_PER_DAY)
            {
                // Filter cycle wraps past midnight
                // Running if: current >= start OR current < (end - MINUTES_PER_DAY)
                return (current_minutes >= start_minutes) || (current_minutes < (end_minutes - MINUTES_PER_DAY));
            }
            else
            {
                // Normal case: filter cycle within same day
                // Running if: start <= current < end
                return (current_minutes >= start_minutes) && (current_minutes < end_minutes);
            }
        }

        void BalboaSpaBinarySensors::set_parent(BalboaSpa *parent)
        {
            this->spa = parent;
            parent->register_listener([this](SpaState *spaState)
                                      { this->update(spaState); });
        }

        void BalboaSpaBinarySensors::update(SpaState *spaState)
        {
            bool sensor_state_value;
            if (spa == nullptr || spaState == nullptr || (!spa->is_communicating() && sensor_type != BalboaSpaBinarySensorType::CONNECTED))
            {
                this->publish_state(NAN);
                return;
            }

            // Get filter settings once if needed for filter-related sensors
            SpaFilterSettings *filterSettings = nullptr;
            if (sensor_type == BalboaSpaBinarySensorType::FILTER1_RUNNING ||
                sensor_type == BalboaSpaBinarySensorType::FILTER2_RUNNING)
            {
                filterSettings = spa->get_current_filter_settings();
                if (filterSettings == nullptr)
                {
                    // Filter settings not available yet
                    this->publish_state(NAN);
                    return;
                }
            }

            uint8_t state_value = 0;
            switch (sensor_type)
            {
            case BalboaSpaBinarySensorType::BLOWER:
                sensor_state_value = spaState->blower;
                break;
            case BalboaSpaBinarySensorType::HIGHRANGE:
                sensor_state_value = spaState->highrange;
                break;
            case BalboaSpaBinarySensorType::CIRCULATION:
                sensor_state_value = spaState->circulation;
                break;
            case BalboaSpaBinarySensorType::RESTMODE:
                state_value = spaState->rest_mode;
                sensor_state_value = state_value;
                if (state_value == 254)
                {
                    // This indicate no value
                    return;
                }
                break;
            case BalboaSpaBinarySensorType::HEATSTATE:
                state_value = spaState->heat_state;
                sensor_state_value = state_value;
                if (state_value == 254)
                {
                    // no value
                    return;
                }
                break;
            case BalboaSpaBinarySensorType::CONNECTED:
                sensor_state_value = spa->is_communicating();
                break;
            case BalboaSpaBinarySensorType::FILTER1_RUNNING:
            {
                sensor_state_value = is_filter_running(
                    spaState->hour, spaState->minutes,
                    filterSettings->filter1_hour, filterSettings->filter1_minute,
                    filterSettings->filter1_duration_hour, filterSettings->filter1_duration_minute);
                break;
            }
            case BalboaSpaBinarySensorType::FILTER2_RUNNING:
            {
                // Filter 2 can only be running if it's enabled
                if (filterSettings->filter2_enable)
                {
                    sensor_state_value = is_filter_running(
                        spaState->hour, spaState->minutes,
                        filterSettings->filter2_hour, filterSettings->filter2_minute,
                        filterSettings->filter2_duration_hour, filterSettings->filter2_duration_minute);
                }
                else
                {
                    sensor_state_value = false;
                }
                break;
            }
            default:
                ESP_LOGD(TAG, "Spa/BSensors/UnknownSensorType: SensorType Number: %d", sensor_type);
                // Unknown enum value. Ignore
                return;
            }

            if (this->state != sensor_state_value || this->last_update_time + 300000 < millis())
            {
                this->publish_state(sensor_state_value);
                last_update_time = millis();
            }
        }

        BalboaSpaBinarySensors::BalboaSpaBinarySensors()
        {
            spa = nullptr;
            sensor_type = BalboaSpaBinarySensorType::UNKNOWN;
            last_update_time = 0;
        }

    }
}
