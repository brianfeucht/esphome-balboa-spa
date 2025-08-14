#include "filter2_enable_switch.h"

namespace esphome {
namespace balboa_spa {

static const char *TAG = "BalboaSpa.filter2_enable_switch";

void Filter2EnableSwitch::update(const SpaFilterSettings* filter_settings) {
    bool enabled = filter_settings->filter2_enable == 1;
    if (this->state != enabled) {
        this->publish_state(enabled);
        ESP_LOGD(TAG, "Filter2 enable switch state updated to %s", enabled ? STRON : STROFF);
    }
}

void Filter2EnableSwitch::set_parent(BalboaSpa *parent) {
    spa = parent;
    // Note: We'll need to handle filter settings updates differently since they're not as frequent
    // For now, this will be updated when filter settings are decoded
}

void Filter2EnableSwitch::write_state(bool state) {
    ESP_LOGD(TAG, "Filter2 enable set to %s", state ? STRON : STROFF);
    spa->set_filter2_enabled(state);
    // Update the state immediately since we initiated the change
    this->publish_state(state);
}

}  // namespace balboa_spa
}  // namespace esphome