#include "esphome/core/log.h"
#include "sensors.h"

namespace esphome {
namespace balboa_spa {

void BalboaSpaSensors::setup() {

}

void BalboaSpaSensors::update() {
    SpaState* spaState = spa->get_current_state();

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
        default:
            // Unknown enum value. Ignore
            return;
    }

    if(this->state != sensor_state_value)
    {
        this->publish_state(sensor_state_value);
    }
}

void BalboaSpaSensors::dump_config() {
  LOG_SENSOR("", "Balboa Spa Sensor", this);
  ESP_LOGCONFIG(TAG, "  Sensor has datapoint ID %u", this->sensor_id_);
}
}}