#pragma once

#include "esphome/components/text_sensor/text_sensor.h"
#include "../balboaspa.h"

namespace esphome {
namespace balboa_spa {

class SpaFilter1ConfigTextSensor : public text_sensor::TextSensor {
 public:
  void set_parent(BalboaSpa *parent);
  void update(SpaFilterSettings* filterSettings);
 private:
  BalboaSpa *parent_;
  // Store last known values for change detection
  uint8_t last_filter1_hour_ = 255;
  uint8_t last_filter1_minute_ = 255;
  uint8_t last_filter1_duration_hour_ = 255;
  uint8_t last_filter1_duration_minute_ = 255;
};

class SpaFilter2ConfigTextSensor : public text_sensor::TextSensor {
 public:
  void set_parent(BalboaSpa *parent);
  void update(SpaFilterSettings* filterSettings);
 private:
  BalboaSpa *parent_;
  // Store last known values for change detection
  uint8_t last_filter2_enable_ = 255;
  uint8_t last_filter2_hour_ = 255;
  uint8_t last_filter2_minute_ = 255;
  uint8_t last_filter2_duration_hour_ = 255;
  uint8_t last_filter2_duration_minute_ = 255;
};

}  // namespace balboa_spa
}  // namespace esphome
