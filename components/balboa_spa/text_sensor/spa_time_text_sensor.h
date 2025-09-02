#pragma once
#include "esphome/components/text_sensor/text_sensor.h"
#include "../balboaspa.h"

namespace esphome {
namespace balboa_spa {

class SpaTimeTextSensor : public text_sensor::TextSensor {
 public:
  void set_parent(BalboaSpa *parent);
  void update(SpaState* spaState);
};

}  // namespace balboa_spa
}  // namespace esphome
