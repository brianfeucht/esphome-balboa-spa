#pragma once
#include "esphome/components/text_sensor/text_sensor.h"
#include "../balboaspa.h"

namespace esphome {
namespace balboa_spa {

class SpaTimeTextSensor : public text_sensor::TextSensor {
 public:
  void set_parent(BalboaSpa *parent);
  void update(SpaState* spaState);
 private:
  // Store last known values for change detection
  uint8_t last_hour_ = 255;
  uint8_t last_minutes_ = 255;
};

}  // namespace balboa_spa
}  // namespace esphome
