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
};

class SpaFilter2ConfigTextSensor : public text_sensor::TextSensor {
 public:
  void set_parent(BalboaSpa *parent);
  void update(SpaFilterSettings* filterSettings);
 private:
  BalboaSpa *parent_;
};

}  // namespace balboa_spa
}  // namespace esphome
