#pragma once

#include "esphome/core/component.h"
#include "esphome/components/fan/fan.h"
#include "../balboaspa.h"

namespace esphome {
namespace balboa_spa {

class JetFan : public fan::Fan, public Component {
 public:
  void setup() override;
  void dump_config() override;
  fan::FanTraits get_traits() override;
  
  void set_parent(BalboaSpa *parent) { this->parent_ = parent; }
  void set_jet_number(int jet_number) { this->jet_number_ = jet_number; }
  void set_jet_name(const std::string &jet_name) { this->jet_name_ = jet_name; }
  
  std::function<void(SpaState *)> get_listener() {
    return [this](SpaState *state) { this->on_spa_state_change(state); };
  }

 protected:
  void control(const fan::FanCall &call) override;
  void on_spa_state_change(SpaState *state);
  
  BalboaSpa *parent_;
  int jet_number_;
  std::string jet_name_;
};

}  // namespace balboa_spa
}  // namespace esphome