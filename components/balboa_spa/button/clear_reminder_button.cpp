#include "clear_reminder_button.h"
#include "esphome/core/log.h"

namespace esphome
{
    namespace balboa_spa
    {
        static const char *TAG = "balboa_spa.button.clear_reminder";

        void ClearReminderButton::set_parent(BalboaSpa *parent)
        {
            parent_ = parent;
        }

        void ClearReminderButton::press_action()
        {
            ESP_LOGI(TAG, "Clear reminder button pressed");
            parent_->clear_reminder();
        }

    } // namespace balboa_spa
} // namespace esphome
