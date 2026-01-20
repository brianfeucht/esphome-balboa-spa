#include "request_fault_log_button.h"
#include "esphome/core/log.h"

namespace esphome
{
    namespace balboa_spa
    {
        static const char *TAG = "balboa_spa.button";

        void RequestFaultLogButton::set_parent(BalboaSpa *parent)
        {
            parent_ = parent;
        }

        void RequestFaultLogButton::press_action()
        {
            ESP_LOGD(TAG, "Request fault log button pressed");
            parent_->request_fault_log_update();
            ESP_LOGI(TAG, "Fault log update requested");
        }

    } // namespace balboa_spa
} // namespace esphome
