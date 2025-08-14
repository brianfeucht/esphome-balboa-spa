#include "esphome/core/log.h"
#include "numbers.h"

namespace esphome {
namespace balboa_spa {

static const char *TAG = "BalboaSpa.numbers";

void SpaHourNumber::control(float value) {
    int hour = (int)value;
    ESP_LOGD(TAG, "Spa/hour/set: %d", hour);
    this->parent_->set_hour(hour);
    this->publish_state(value);
}

void SpaHourNumber::update_from_spa_state(const SpaState* spa_state) {
    float hour = (float)spa_state->hour;
    if (this->state != hour) {
        this->publish_state(hour);
    }
}

void SpaMinuteNumber::control(float value) {
    int minute = (int)value;
    ESP_LOGD(TAG, "Spa/minute/set: %d", minute);
    this->parent_->set_minute(minute);
    this->publish_state(value);
}

void SpaMinuteNumber::update_from_spa_state(const SpaState* spa_state) {
    float minute = (float)spa_state->minutes;
    if (this->state != minute) {
        this->publish_state(minute);
    }
}

void Filter1StartHourNumber::control(float value) {
    int hour = (int)value;
    ESP_LOGD(TAG, "Filter1/start_hour/set: %d", hour);
    auto* filter_settings = this->parent_->get_current_filter_settings();
    this->parent_->set_filter1_start_time(hour, filter_settings->filter1_minute);
    this->publish_state(value);
}

void Filter1StartHourNumber::update_from_filter_settings(const SpaFilterSettings* filter_settings) {
    float hour = (float)filter_settings->filter1_hour;
    if (this->state != hour) {
        this->publish_state(hour);
    }
}

void Filter1StartMinuteNumber::control(float value) {
    int minute = (int)value;
    ESP_LOGD(TAG, "Filter1/start_minute/set: %d", minute);
    auto* filter_settings = this->parent_->get_current_filter_settings();
    this->parent_->set_filter1_start_time(filter_settings->filter1_hour, minute);
    this->publish_state(value);
}

void Filter1StartMinuteNumber::update_from_filter_settings(const SpaFilterSettings* filter_settings) {
    float minute = (float)filter_settings->filter1_minute;
    if (this->state != minute) {
        this->publish_state(minute);
    }
}

void Filter1DurationHourNumber::control(float value) {
    int hour = (int)value;
    ESP_LOGD(TAG, "Filter1/duration_hour/set: %d", hour);
    auto* filter_settings = this->parent_->get_current_filter_settings();
    this->parent_->set_filter1_duration(hour, filter_settings->filter1_duration_minute);
    this->publish_state(value);
}

void Filter1DurationHourNumber::update_from_filter_settings(const SpaFilterSettings* filter_settings) {
    float hour = (float)filter_settings->filter1_duration_hour;
    if (this->state != hour) {
        this->publish_state(hour);
    }
}

void Filter1DurationMinuteNumber::control(float value) {
    int minute = (int)value;
    ESP_LOGD(TAG, "Filter1/duration_minute/set: %d", minute);
    auto* filter_settings = this->parent_->get_current_filter_settings();
    this->parent_->set_filter1_duration(filter_settings->filter1_duration_hour, minute);
    this->publish_state(value);
}

void Filter1DurationMinuteNumber::update_from_filter_settings(const SpaFilterSettings* filter_settings) {
    float minute = (float)filter_settings->filter1_duration_minute;
    if (this->state != minute) {
        this->publish_state(minute);
    }
}

void Filter2StartHourNumber::control(float value) {
    int hour = (int)value;
    ESP_LOGD(TAG, "Filter2/start_hour/set: %d", hour);
    auto* filter_settings = this->parent_->get_current_filter_settings();
    this->parent_->set_filter2_start_time(hour, filter_settings->filter2_minute);
    this->publish_state(value);
}

void Filter2StartHourNumber::update_from_filter_settings(const SpaFilterSettings* filter_settings) {
    float hour = (float)filter_settings->filter2_hour;
    if (this->state != hour) {
        this->publish_state(hour);
    }
}

void Filter2StartMinuteNumber::control(float value) {
    int minute = (int)value;
    ESP_LOGD(TAG, "Filter2/start_minute/set: %d", minute);
    auto* filter_settings = this->parent_->get_current_filter_settings();
    this->parent_->set_filter2_start_time(filter_settings->filter2_hour, minute);
    this->publish_state(value);
}

void Filter2StartMinuteNumber::update_from_filter_settings(const SpaFilterSettings* filter_settings) {
    float minute = (float)filter_settings->filter2_minute;
    if (this->state != minute) {
        this->publish_state(minute);
    }
}

void Filter2DurationHourNumber::control(float value) {
    int hour = (int)value;
    ESP_LOGD(TAG, "Filter2/duration_hour/set: %d", hour);
    auto* filter_settings = this->parent_->get_current_filter_settings();
    this->parent_->set_filter2_duration(hour, filter_settings->filter2_duration_minute);
    this->publish_state(value);
}

void Filter2DurationHourNumber::update_from_filter_settings(const SpaFilterSettings* filter_settings) {
    float hour = (float)filter_settings->filter2_duration_hour;
    if (this->state != hour) {
        this->publish_state(hour);
    }
}

void Filter2DurationMinuteNumber::control(float value) {
    int minute = (int)value;
    ESP_LOGD(TAG, "Filter2/duration_minute/set: %d", minute);
    auto* filter_settings = this->parent_->get_current_filter_settings();
    this->parent_->set_filter2_duration(filter_settings->filter2_duration_hour, minute);
    this->publish_state(value);
}

void Filter2DurationMinuteNumber::update_from_filter_settings(const SpaFilterSettings* filter_settings) {
    float minute = (float)filter_settings->filter2_duration_minute;
    if (this->state != minute) {
        this->publish_state(minute);
    }
}

}  // namespace balboa_spa
}  // namespace esphome