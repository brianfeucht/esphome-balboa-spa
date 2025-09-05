#include "fault_log_dump_button.h"
#include "esphome/core/log.h"

namespace esphome
{
    namespace balboa_spa
    {
        static const char *TAG = "balboa_spa.button";

        void FaultLogDumpButton::set_parent(BalboaSpa *parent)
        {
            parent_ = parent;
        }

        void FaultLogDumpButton::press_action()
        {
            ESP_LOGI(TAG, "Fault log dump button pressed");
            
            if (parent_ == nullptr) {
                ESP_LOGW(TAG, "Parent BalboaSpa is null, cannot dump fault log");
                return;
            }
            
            // Trigger fault log dump process
            parent_->start_fault_log_dump();
            ESP_LOGI(TAG, "Fault log dump started");
        }

    } // namespace balboa_spa
} // namespace esphome