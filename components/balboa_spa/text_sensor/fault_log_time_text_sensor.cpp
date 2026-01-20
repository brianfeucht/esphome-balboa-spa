#include "fault_log_time_text_sensor.h"
#include <time.h>

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
                // Calculate ISO 8601 timestamp from current time minus days_ago
                time_t now_timestamp = time(nullptr);
                
                // Subtract days_ago from current time
                time_t fault_timestamp = now_timestamp - (spaFaultLog->days_ago * 86400); // 86400 seconds per day
                
                // Get the date part from the calculated timestamp
                struct tm *time_info = localtime(&fault_timestamp);
                
                if (time_info != nullptr && now_timestamp >= 1000000000) // Basic validity check
                {
                    // Set the specific hour and minute from the fault log
                    time_info->tm_hour = spaFaultLog->hour;
                    time_info->tm_min = spaFaultLog->minutes;
                    time_info->tm_sec = 0;
                    
                    // Convert back to timestamp
                    fault_timestamp = mktime(time_info);
                    
                    // Get the time_info again for the adjusted timestamp
                    time_info = localtime(&fault_timestamp);
                    
                    // Format as ISO 8601: YYYY-MM-DDTHH:MM:SS
                    // Buffer size: "2026-01-20T15:52:21" (19 chars) + null terminator
                    char buf[32];
                    strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", time_info);
                    this->publish_state(buf);
                }
                else
                {
                    // System time not valid yet, publish placeholder
                    this->publish_state("N/A");
                }

                // Update last known values
                last_days_ago_ = spaFaultLog->days_ago;
                last_hour_ = spaFaultLog->hour;
                last_minutes_ = spaFaultLog->minutes;
            }
        }

    } // namespace balboa_spa
} // namespace esphome
