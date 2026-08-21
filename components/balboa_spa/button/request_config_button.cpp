#include "request_config_button.h"
#include "esphome/core/log.h"

namespace esphome
{
    namespace balboa_spa
    {
        static const char *TAG = "balboa_spa.button";

        void RequestConfigButton::set_parent(BalboaSpa *parent)
        {
            parent_ = parent;
        }

        void RequestConfigButton::press_action()
        {
            ESP_LOGD(TAG, "Request config button pressed");
            parent_->request_config_update();
            ESP_LOGI(TAG, "Spa config update requested");
        }

    } // namespace balboa_spa
} // namespace esphome
