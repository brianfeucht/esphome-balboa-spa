#include "spa_time_text_sensor.h"

namespace esphome {
namespace balboa_spa {

void SpaTimeTextSensor::set_parent(BalboaSpa *parent) {
    parent->register_listener([this](SpaState* spaState){ this->update(spaState); });
}

void SpaTimeTextSensor::update(SpaState* spaState) {
    // Check if time has changed
    if (spaState->hour != last_hour_ || spaState->minutes != last_minutes_) {
        char buf[6];
        snprintf(buf, sizeof(buf), "%02u:%02u", spaState->hour, spaState->minutes);
        this->publish_state(buf);
        
        // Update last known values
        last_hour_ = spaState->hour;
        last_minutes_ = spaState->minutes;
    }
}

}  // namespace balboa_spa
}  // namespace esphome
