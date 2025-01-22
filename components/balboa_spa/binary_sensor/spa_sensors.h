#pragma once

#include "esphome/core/component.h"

#include "esphome/components/binary_sensor/binary_sensor.h"
#include "../balboaspa.h"

namespace esphome {
namespace balboa_spa {

class BalboaSpaSensors :  public PollingComponent {
 public:
  // Update thermostat status from UART device every second
  BalboaSpaSensors() : PollingComponent(1000) {}

  void update() override;
  void set_parent(BalboaSpa *parent);
  
  void set_jet1_binary_sensor(binary_sensor::BinarySensor *sensor);    
  void set_jet2_binary_sensor(binary_sensor::BinarySensor *sensor);
  void set_lights_binary_sensor(binary_sensor::BinarySensor *sensor);
 private:
  BalboaSpa *spa;
  binary_sensor::BinarySensor *jet1_sensor;
  binary_sensor::BinarySensor *jet2_sensor;
  binary_sensor::BinarySensor *lights_sensor;

  void update_sensors();
};

}  // namespace balboa_spa
}  // namespace esphome
