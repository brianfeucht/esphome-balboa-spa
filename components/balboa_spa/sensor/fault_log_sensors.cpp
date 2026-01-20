#include "esphome/core/log.h"
#include "fault_log_sensors.h"

namespace esphome
{
    namespace balboa_spa
    {

        static const char *TAG = "BalboaSpa.fault_log_sensors";

        void BalboaSpaFaultLogSensors::set_parent(BalboaSpa *parent)
        {
            parent->register_fault_log_listener([this](SpaFaultLog *spaFaultLog)
                                      { this->update(spaFaultLog); });
        }

        void BalboaSpaFaultLogSensors::update(SpaFaultLog *spaFaultLog)
        {
            uint8_t sensor_state_value = 0; // Initialize to avoid undefined behavior

            switch (sensor_type)
            {
            case BalboaSpaFaultLogSensorType::FAULT_CODE:
                sensor_state_value = spaFaultLog->fault_code;
                break;
            case BalboaSpaFaultLogSensorType::FAULT_TOTAL_ENTRIES:
                sensor_state_value = spaFaultLog->total_entries;
                break;
            case BalboaSpaFaultLogSensorType::FAULT_CURRENT_ENTRY:
                sensor_state_value = spaFaultLog->current_entry;
                break;
            case BalboaSpaFaultLogSensorType::FAULT_DAYS_AGO:
                sensor_state_value = spaFaultLog->days_ago;
                break;
            default:
                ESP_LOGD(TAG, "Spa/FaultLogSensors/UnknownSensorType: SensorType Number: %u", static_cast<uint8_t>(sensor_type));
                // Unknown enum value. Ignore
                return;
            }

            if (this->state != sensor_state_value)
            {
                this->publish_state(sensor_state_value);
            }
        }
    }
}
