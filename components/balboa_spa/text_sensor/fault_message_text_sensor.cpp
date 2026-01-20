#include "fault_message_text_sensor.h"

namespace esphome
{
    namespace balboa_spa
    {

        void FaultMessageTextSensor::set_parent(BalboaSpa *parent)
        {
            parent->register_fault_log_listener(
                [this](SpaFaultLog *spaFaultLog)
                {
                    this->update(spaFaultLog);
                });
        }

        void FaultMessageTextSensor::update(SpaFaultLog *spaFaultLog)
        {
            // Check if the fault message has changed
            if (spaFaultLog->fault_message != last_message_)
            {
                this->publish_state(spaFaultLog->fault_message);
                last_message_ = spaFaultLog->fault_message;
            }
        }

    } // namespace balboa_spa
} // namespace esphome
