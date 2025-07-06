#include "esphome/core/log.h"
#include "binary_sensors.h"

namespace esphome {
namespace balboa_spa {

static const char *TAG = "BalboaSpa.binary_sensors";

void BalboaSpaBinarySensors::set_parent(BalboaSpa *parent) {
    this->spa = parent;
    parent->register_listener([this](SpaState* spaState){ this->update(spaState); });
}

void BalboaSpaBinarySensors::update(SpaState* spaState) {
    bool sensor_state_value;
    if (spa == nullptr || ( !spa->is_communicating() && sensor_type != BalboaSpaBinarySensorType::CONNECTED )){
        this->publish_state(NAN);
        return;
    }

    uint8_t d = 0;
    switch (sensor_type)
    {
        case BalboaSpaBinarySensorType::BLOWER:
            sensor_state_value = spaState->blower;
            break;
        case BalboaSpaBinarySensorType::HIGHRANGE:
            sensor_state_value = spaState->highrange;
            break;
        case BalboaSpaBinarySensorType::CIRCULATION:
            sensor_state_value = spaState->circulation;
            break;
        case BalboaSpaBinarySensorType::RESTMODE:
            d = spaState->rest_mode;
            sensor_state_value = d;
            if(d == 254) {
                // This indicate no value
                return;
            }
            break;
        case BalboaSpaBinarySensorType::HEATSTATE:
            d = spaState->heat_state;
            sensor_state_value = d;
            if(d == 254) {
                // no value
                return;
            }
            break;
        case BalboaSpaBinarySensorType::CONNECTED:
            sensor_state_value = spa->is_communicating();
            break;
        default:
            ESP_LOGD(TAG, "Spa/BSensors/UnknownSensorType: SensorType Number: %d", sensor_type);
            // Unknown enum value. Ignore
            return;
    }

    if(this->state != sensor_state_value || this->lastUpdate + 300000 < millis()) {
        this->publish_state(sensor_state_value);
        lastUpdate = millis();
    }
}

BalboaSpaBinarySensors::BalboaSpaBinarySensors() {
    spa = nullptr;
    sensor_type = BalboaSpaBinarySensorType::UNKNOWN;
    lastUpdate = 0;
}

}}
