#include "spa_time_text_sensor.h"

namespace esphome {
namespace balboa_spa {

void SpaTimeTextSensor::set_parent(BalboaSpa *parent) {
    parent->register_listener([this](SpaState* spaState){ this->update(spaState); });
}

void SpaTimeTextSensor::update(SpaState* spaState) {
    char buf[6];
    snprintf(buf, sizeof(buf), "%02u:%02u", spaState->hour, spaState->minutes);
    this->publish_state(buf);
}

}  // namespace balboa_spa
}  // namespace esphome
