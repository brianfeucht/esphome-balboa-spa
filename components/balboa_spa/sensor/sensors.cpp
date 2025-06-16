#include "esphome/core/log.h"
#include "sensors.h"

namespace esphome {
namespace balboa_spa {

BalboaSpaSensors::BalboaSpaSensors() {
    this->spa = nullptr;
    this->sensor_type = BalboaSpaSensorType::UNKNOWN;
}

void BalboaSpaSensors::set_parent(BalboaSpa *parent) {
    this->spa = parent;
    parent->register_listener([this](SpaState* spaState){ this->update(spaState); });
}

void BalboaSpaSensors::update(SpaState* spaState) {
    uint8_t sensor_state_value;
    if ( spa == nullptr || !spa->is_communicating() && sensor_type != BalboaSpaSensorType::CONNECTED){
        this->publish_state(NAN);
        return;
    }

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
        case BalboaSpaSensorType::CONNECTED:
            sensor_state_value = spa != nullptr && spa->is_communicating();
            break;
        case BalboaSpaSensorType::SENSOR_A:
            sensor_state_value = spaState->sensor_a;
            break;
        case BalboaSpaSensorType::SENSOR_B:
            sensor_state_value = spaState->sensor_b;
            break;
        default:
            ESP_LOGD("Spa/Sensors/UnknownSensorType", "SensorType Number: %d", sensor_type);
            // Unknown enum value. Ignore
            return;
    }

    if(this->state != sensor_state_value || this->lastUpdate + 300000 < millis()) {
        this->publish_state(sensor_state_value);
        this->lastUpdate = millis();
    }
}
}}