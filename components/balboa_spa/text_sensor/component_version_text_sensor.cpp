#include "component_version_text_sensor.h"
#include "../version.h"

namespace esphome
{
    namespace balboa_spa
    {
        static const char *TAG = "balboa_spa.text_sensor.component_version";

        void ComponentVersionTextSensor::set_parent(BalboaSpa *parent)
        {
            ESP_LOGD(TAG, "Component version: %s", BALBOA_SPA_VERSION);
            this->publish_state(BALBOA_SPA_VERSION);
        }

    } // namespace balboa_spa
} // namespace esphome
