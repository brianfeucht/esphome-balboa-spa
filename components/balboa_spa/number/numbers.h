#pragma once

#include "esphome/core/component.h"
#include "esphome/components/number/number.h"
#include "../balboaspa.h"

namespace esphome {
namespace balboa_spa {

class BalboaSpaNumber : public number::Number, public Component {
  public:
    void set_parent(BalboaSpa *parent) { 
        this->parent_ = parent; 
        parent->register_number_entity(this);
    }
    virtual void update_from_filter_settings(const SpaFilterSettings* filter_settings) {}
    virtual void update_from_spa_state(const SpaState* spa_state) {}

  protected:
    BalboaSpa *parent_;
};

class SpaHourNumber : public BalboaSpaNumber {
  public:
    void update_from_spa_state(const SpaState* spa_state);
  protected:
    void control(float value) override;
};

class SpaMinuteNumber : public BalboaSpaNumber {
  public:
    void update_from_spa_state(const SpaState* spa_state);
  protected:
    void control(float value) override;
};

class Filter1StartHourNumber : public BalboaSpaNumber {
  public:
    void update_from_filter_settings(const SpaFilterSettings* filter_settings) override;
  protected:
    void control(float value) override;
};

class Filter1StartMinuteNumber : public BalboaSpaNumber {
  public:
    void update_from_filter_settings(const SpaFilterSettings* filter_settings) override;
  protected:
    void control(float value) override;
};

class Filter1DurationHourNumber : public BalboaSpaNumber {
  public:
    void update_from_filter_settings(const SpaFilterSettings* filter_settings) override;
  protected:
    void control(float value) override;
};

class Filter1DurationMinuteNumber : public BalboaSpaNumber {
  public:
    void update_from_filter_settings(const SpaFilterSettings* filter_settings) override;
  protected:
    void control(float value) override;
};

class Filter2StartHourNumber : public BalboaSpaNumber {
  public:
    void update_from_filter_settings(const SpaFilterSettings* filter_settings) override;
  protected:
    void control(float value) override;
};

class Filter2StartMinuteNumber : public BalboaSpaNumber {
  public:
    void update_from_filter_settings(const SpaFilterSettings* filter_settings) override;
  protected:
    void control(float value) override;
};

class Filter2DurationHourNumber : public BalboaSpaNumber {
  public:
    void update_from_filter_settings(const SpaFilterSettings* filter_settings) override;
  protected:
    void control(float value) override;
};

class Filter2DurationMinuteNumber : public BalboaSpaNumber {
  public:
    void update_from_filter_settings(const SpaFilterSettings* filter_settings) override;
  protected:
    void control(float value) override;
};

}  // namespace balboa_spa
}  // namespace esphome