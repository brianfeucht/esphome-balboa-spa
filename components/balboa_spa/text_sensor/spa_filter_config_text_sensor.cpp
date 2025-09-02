#include "spa_filter_config_text_sensor.h"

namespace esphome {
namespace balboa_spa {

void SpaFilter1ConfigTextSensor::set_parent(BalboaSpa *parent) {
    this->parent_ = parent;
    parent->register_listener([this](SpaState* spaState){ 
        auto filterSettings = this->parent_->get_current_filter_settings();
        this->update(filterSettings); 
    });
}

void SpaFilter1ConfigTextSensor::update(SpaFilterSettings* filterSettings) {
    // Format: {"start":"HH:MM","duration":"HH:MM"}
    char buf[50];
    snprintf(buf, sizeof(buf), "{\"start\":\"%02u:%02u\",\"duration\":\"%02u:%02u\"}", 
             filterSettings->filter1_hour, 
             filterSettings->filter1_minute,
             filterSettings->filter1_duration_hour, 
             filterSettings->filter1_duration_minute);
    this->publish_state(buf);
}

void SpaFilter2ConfigTextSensor::set_parent(BalboaSpa *parent) {
    this->parent_ = parent;
    parent->register_listener([this](SpaState* spaState){ 
        auto filterSettings = this->parent_->get_current_filter_settings();
        this->update(filterSettings); 
    });
}

void SpaFilter2ConfigTextSensor::update(SpaFilterSettings* filterSettings) {
    if (filterSettings->filter2_enable) {
        // Format: {"start":"HH:MM","duration":"HH:MM"}
        char buf[50];
        snprintf(buf, sizeof(buf), "{\"start\":\"%02u:%02u\",\"duration\":\"%02u:%02u\"}", 
                 filterSettings->filter2_hour, 
                 filterSettings->filter2_minute,
                 filterSettings->filter2_duration_hour, 
                 filterSettings->filter2_duration_minute);
        this->publish_state(buf);
    } else {
        this->publish_state("disabled");
    }
}

}  // namespace balboa_spa
}  // namespace esphome
