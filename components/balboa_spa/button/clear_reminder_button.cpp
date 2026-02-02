#include "clear_reminder_button.h"

namespace esphome
{
    namespace balboa_spa
    {

        void ClearReminderButton::set_parent(BalboaSpa *parent)
        {
            parent_ = parent;
        }

        void ClearReminderButton::press_action()
        {
            parent_->clear_reminder();
        }

    } // namespace balboa_spa
} // namespace esphome
