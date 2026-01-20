#include "fault_log_time_text_sensor.h"

namespace esphome
{
    namespace balboa_spa
    {

        void FaultLogTimeTextSensor::set_parent(BalboaSpa *parent)
        {
            parent->register_fault_log_listener(
                [this](SpaFaultLog *spaFaultLog)
                {
                    this->update(spaFaultLog);
                });
        }

        void FaultLogTimeTextSensor::update(SpaFaultLog *spaFaultLog)
        {
            // Check if time has changed
            if (spaFaultLog->days_ago != last_days_ago_ || 
                spaFaultLog->hour != last_hour_ || 
                spaFaultLog->minutes != last_minutes_)
            {
                char buf[32];
                if (spaFaultLog->days_ago == 0)
                {
                    snprintf(buf, sizeof(buf), "Today %02u:%02u", spaFaultLog->hour, spaFaultLog->minutes);
                }
                else if (spaFaultLog->days_ago == 1)
                {
                    snprintf(buf, sizeof(buf), "Yesterday %02u:%02u", spaFaultLog->hour, spaFaultLog->minutes);
                }
                else
                {
                    snprintf(buf, sizeof(buf), "%u days ago %02u:%02u", spaFaultLog->days_ago, spaFaultLog->hour, spaFaultLog->minutes);
                }
                this->publish_state(buf);

                // Update last known values
                last_days_ago_ = spaFaultLog->days_ago;
                last_hour_ = spaFaultLog->hour;
                last_minutes_ = spaFaultLog->minutes;
            }
        }

    } // namespace balboa_spa
} // namespace esphome
