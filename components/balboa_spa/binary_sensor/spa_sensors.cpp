#include "esphome.h"
#include "esphome/core/log.h"
#include "spa_sensors.h"

namespace esphome {
namespace balboa_spa {

void BalboaSpaSensors::set_parent(BalboaSpa *parent) { spa = parent; }

void BalboaSpaSensors::update() {
  ESP_LOGD("SPA/Sensors", "Starting update");
  if(spa != nullptr)
  {
    update_sensors();
  }
  else
  {
    ESP_LOGD("SPA/Sensors", "Parent not yet associated.  Unable to update");
  }
}

void BalboaSpaSensors::set_jet1_binary_sensor(binary_sensor::BinarySensor *sensor) { 
    jet1_sensor = sensor;
    jet1_sensor->add_on_state_callback([this](float state) {
        if(jet1_sensor->state != state){
            spa->toggle_jet1();
        }
    });
}

void BalboaSpaSensors::set_jet2_binary_sensor(binary_sensor::BinarySensor *sensor) {
    jet2_sensor = sensor;
    jet2_sensor->add_on_state_callback([this](float state) {
        if(jet2_sensor->state != state){
            spa->toggle_jet2();
        }
    });
}
    
void BalboaSpaSensors::set_lights_binary_sensor(binary_sensor::BinarySensor *sensor) {
    lights_sensor = sensor;
    
    lights_sensor->add_on_state_callback([this](float state) {
        if(lights_sensor->state != state){
            spa->toggle_light();
        }
    });
}

void BalboaSpaSensors::update_sensors() {
    ESP_LOGD("SPA/Sensors", "Requested SPA State");
    SpaState spaState; // = spa->get_current_state();

    ESP_LOGD("SPA/Sensors", "Received SPA State");
    
    if(this->jet1_sensor != nullptr && 
       this->jet1_sensor->state != spaState.jet1)
    {
      jet1_sensor->publish_state(spaState.jet1);
    }
    
    if(this->jet2_sensor != nullptr &&
       this->jet1_sensor->state != spaState.jet2)
    {
      jet2_sensor->publish_state(spaState.jet2);
    }

    if(this->lights_sensor != nullptr &&
       this->lights_sensor->state != spaState.light)
    {
      lights_sensor->publish_state(spaState.light);
    }
}
}
}