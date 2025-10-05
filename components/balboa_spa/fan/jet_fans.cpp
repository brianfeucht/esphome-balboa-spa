#include "jet_fans.h"
#include "esphome/core/log.h"

namespace esphome {
namespace balboa_spa {

static const char *TAG = "BalboaSpa.jet_fan";

// Helper function to get fan traits (same for all jets)
fan::FanTraits get_jet_fan_traits() {
  fan::FanTraits traits;
  traits.set_oscillation(false);
  traits.set_direction(false);
  traits.set_speed(true);
  traits.set_supported_speed_count(2);  // LOW and HIGH (OFF is handled by on/off)
  return traits;
}

// Helper function for fan control logic
void control_jet_fan(BalboaSpa *parent, const fan::FanCall &call, int jet_number, const char* jet_name) {
  if (parent == nullptr) {
    ESP_LOGW(TAG, "No parent spa component set for %s", jet_name);
    return;
  }
  
  if (call.get_state().has_value()) {
    bool target_state = *call.get_state();
    
    if (target_state) {
      // Fan is being turned on - check if speed is specified
      uint8_t jet_speed = 1;  // Default to LOW speed
      
      if (call.get_speed().has_value()) {
        int fan_speed = *call.get_speed();
        // ESPHome fan speeds: 1=LOW, 2=HIGH
        // Balboa jet speeds: 0=OFF, 1=LOW, 2=HIGH  
        jet_speed = fan_speed;  // Direct mapping: fan_speed 1->jet_speed 1, fan_speed 2->jet_speed 2
      }
      
      ESP_LOGD(TAG, "Setting %s to speed %d (fan_speed=%d)", jet_name, jet_speed, 
               call.get_speed().has_value() ? *call.get_speed() : 1);
      
      switch (jet_number) {
        case 1: parent->set_jet1_speed(jet_speed); break;
        case 2: parent->set_jet2_speed(jet_speed); break;
        case 3: parent->set_jet3_speed(jet_speed); break;
        case 4: parent->set_jet4_speed(jet_speed); break;
      }
    } else {
      // Fan is being turned off
      ESP_LOGD(TAG, "Turning off %s", jet_name);
      switch (jet_number) {
        case 1: parent->set_jet1_speed(0); break;
        case 2: parent->set_jet2_speed(0); break;
        case 3: parent->set_jet3_speed(0); break;
        case 4: parent->set_jet4_speed(0); break;
      }
    }
  }
}

// Helper function for state change logic  
void update_jet_fan_state(fan::Fan *fan, SpaState *state, int jet_number, const char* jet_name) {
  if (state == nullptr) return;
  
  uint8_t jet_speed = 0;
  switch (jet_number) {
    case 1: jet_speed = state->jet1; break;
    case 2: jet_speed = state->jet2; break;
    case 3: jet_speed = state->jet3; break;
    case 4: jet_speed = state->jet4; break;
  }
  
  // Update fan state based on jet speed
  // Balboa jet speeds: 0=OFF, 1=LOW, 2=HIGH
  // ESPHome fan speeds: 1=LOW, 2=HIGH (0 is not valid, OFF is handled by state=false)
  bool is_on = (jet_speed > 0);
  int fan_speed = (jet_speed > 0) ? jet_speed : 1;  // Use jet_speed directly as fan_speed, default to 1 when off
  
  // Only update and publish state if it actually changed
  bool state_changed = (fan->state != is_on) || (fan->speed != fan_speed);
  
  if (state_changed) {
    ESP_LOGD(TAG, "%s state changed: jet_speed=%d, fan_on=%d, fan_speed=%d", 
             jet_name, jet_speed, is_on, fan_speed);
    
    // Update fan state without triggering control() again
    // Use direct state assignment to avoid feedback loop
    fan->state = is_on;
    fan->speed = fan_speed;  // Always set speed, even when off (ESPHome expects this)
    fan->publish_state();
  }
}

// ==================== JET 1 FAN ====================

void Jet1Fan::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Balboa Spa Jet 1 Fan...");
  if (this->parent_ != nullptr) {
    SpaState *state = this->parent_->get_current_state();
    this->on_spa_state_change(state);
  }
}

void Jet1Fan::dump_config() {
  ESP_LOGCONFIG(TAG, "Balboa Spa Jet 1 Fan");
}

fan::FanTraits Jet1Fan::get_traits() {
  return get_jet_fan_traits();
}

void Jet1Fan::control(const fan::FanCall &call) {
  control_jet_fan(this->parent_, call, 1, "Jet1");
}

void Jet1Fan::on_spa_state_change(SpaState *state) {
  update_jet_fan_state(this, state, 1, "Jet1");
}

// ==================== JET 2 FAN ====================

void Jet2Fan::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Balboa Spa Jet 2 Fan...");
  if (this->parent_ != nullptr) {
    SpaState *state = this->parent_->get_current_state();
    this->on_spa_state_change(state);
  }
}

void Jet2Fan::dump_config() {
  ESP_LOGCONFIG(TAG, "Balboa Spa Jet 2 Fan");
}

fan::FanTraits Jet2Fan::get_traits() {
  return get_jet_fan_traits();
}

void Jet2Fan::control(const fan::FanCall &call) {
  control_jet_fan(this->parent_, call, 2, "Jet2");
}

void Jet2Fan::on_spa_state_change(SpaState *state) {
  update_jet_fan_state(this, state, 2, "Jet2");
}

// ==================== JET 3 FAN ====================

void Jet3Fan::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Balboa Spa Jet 3 Fan...");
  if (this->parent_ != nullptr) {
    SpaState *state = this->parent_->get_current_state();
    this->on_spa_state_change(state);
  }
}

void Jet3Fan::dump_config() {
  ESP_LOGCONFIG(TAG, "Balboa Spa Jet 3 Fan");
}

fan::FanTraits Jet3Fan::get_traits() {
  return get_jet_fan_traits();
}

void Jet3Fan::control(const fan::FanCall &call) {
  control_jet_fan(this->parent_, call, 3, "Jet3");
}

void Jet3Fan::on_spa_state_change(SpaState *state) {
  update_jet_fan_state(this, state, 3, "Jet3");
}

// ==================== JET 4 FAN ====================

void Jet4Fan::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Balboa Spa Jet 4 Fan...");
  if (this->parent_ != nullptr) {
    SpaState *state = this->parent_->get_current_state();
    this->on_spa_state_change(state);
  }
}

void Jet4Fan::dump_config() {
  ESP_LOGCONFIG(TAG, "Balboa Spa Jet 4 Fan");
}

fan::FanTraits Jet4Fan::get_traits() {
  return get_jet_fan_traits();
}

void Jet4Fan::control(const fan::FanCall &call) {
  control_jet_fan(this->parent_, call, 4, "Jet4");
}

void Jet4Fan::on_spa_state_change(SpaState *state) {
  update_jet_fan_state(this, state, 4, "Jet4");
}

}  // namespace balboa_spa
}  // namespace esphome