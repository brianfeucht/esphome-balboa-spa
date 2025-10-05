#pragma once

#include "esphome/core/component.h"
#include "esphome/components/fan/fan.h"
#include "../balboaspa.h"

namespace esphome {
namespace balboa_spa {

class Jet1Fan : public fan::Fan, public Component {
 public:
  void setup() override;
  void dump_config() override;
  fan::FanTraits get_traits() override;
  
  void set_parent(BalboaSpa *parent) { this->parent_ = parent; }
  
  std::function<void(SpaState *)> get_listener() {
    return [this](SpaState *state) { this->on_spa_state_change(state); };
  }

 protected:
  void control(const fan::FanCall &call) override;
  void on_spa_state_change(SpaState *state);
  
  BalboaSpa *parent_;
};

class Jet2Fan : public fan::Fan, public Component {
 public:
  void setup() override;
  void dump_config() override;
  fan::FanTraits get_traits() override;
  
  void set_parent(BalboaSpa *parent) { this->parent_ = parent; }
  
  std::function<void(SpaState *)> get_listener() {
    return [this](SpaState *state) { this->on_spa_state_change(state); };
  }

 protected:
  void control(const fan::FanCall &call) override;
  void on_spa_state_change(SpaState *state);
  
  BalboaSpa *parent_;
};

class Jet3Fan : public fan::Fan, public Component {
 public:
  void setup() override;
  void dump_config() override;
  fan::FanTraits get_traits() override;
  
  void set_parent(BalboaSpa *parent) { this->parent_ = parent; }
  
  std::function<void(SpaState *)> get_listener() {
    return [this](SpaState *state) { this->on_spa_state_change(state); };
  }

 protected:
  void control(const fan::FanCall &call) override;
  void on_spa_state_change(SpaState *state);
  
  BalboaSpa *parent_;
};

class Jet4Fan : public fan::Fan, public Component {
 public:
  void setup() override;
  void dump_config() override;
  fan::FanTraits get_traits() override;
  
  void set_parent(BalboaSpa *parent) { this->parent_ = parent; }
  
  std::function<void(SpaState *)> get_listener() {
    return [this](SpaState *state) { this->on_spa_state_change(state); };
  }

 protected:
  void control(const fan::FanCall &call) override;
  void on_spa_state_change(SpaState *state);
  
  BalboaSpa *parent_;
};

}  // namespace balboa_spa
}  // namespace esphome