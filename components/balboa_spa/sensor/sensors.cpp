#include "esphome/core/log.h"
#include "sensors.h"

namespace esphome {
namespace balboa_spa {

void BalboaSpaSensors::set_parent(BalboaSpa *parent) {
    parent->register_listener([this](SpaState* spaState){ this->update(spaState); });
}

void BalboaSpaSensors::update(SpaState* spaState) {
    uint8_t sensor_state_value;

    switch (sensor_type)
    {
        case BalboaSpaSensorType::BLOWER:
            sensor_state_value = spaState->blower;
            break;
        case BalboaSpaSensorType::HIGHRANGE:
            sensor_state_value = spaState->highrange;
            break;
        case BalboaSpaSensorType::CIRCULATION:
            sensor_state_value = spaState->circulation;
            break;
        case BalboaSpaSensorType::RESTMODE:
            sensor_state_value = spaState->get_last_rest_mode();
            if(sensor_state_value == 254)
            {
                // This indicate no value
                return;
            }
            break;
        case BalboaSpaSensorType::HEATSTATE:
            sensor_state_value = spaState->get_last_heat_state();
            if(sensor_state_value == 254)
            {
                // no value
                return;
            }
            break;
        default:
            ESP_LOGD("Spa/Sensors/UnknownSensorType", "%s", String((uint8_t)sensor_type, 0).c_str());
            // Unknown enum value. Ignore
            return;
    }

    if(this->state != sensor_state_value)
    {
        this->publish_state(sensor_state_value);
    }
}
}}
