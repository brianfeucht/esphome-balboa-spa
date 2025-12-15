#include "highrange_switch.h"

namespace esphome
{
    namespace balboa_spa
    {
        static const char *TAG = "BalboaSpa.highrange_switch";

        void HighrangeSwitch::update(const SpaState *spaState)
        {
            if (this->state != spaState->highrange)
            {
                this->publish_state(spaState->highrange);
                ESP_LOGD(TAG, "Switch/update: Highrange switch state updated to %s", spaState->highrange ? "HIGH" : "LOW");
            }
        }

        void HighrangeSwitch::set_parent(BalboaSpa *parent)
        {
            spa = parent;
            parent->register_listener([this](const SpaState *spaState)
                                      { this->update(spaState); });
        }

        void HighrangeSwitch::write_state(bool state)
        {
            ESP_LOGD(TAG, "Switch/write_state: Setting highrange to %s", state ? "HIGH" : "LOW");
            spa->set_highrange(state);
        }

    } // namespace balboa_spa
} // namespace esphome
