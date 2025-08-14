#pragma once

#include "esphome/core/component.h"
#include "esphome/components/number/number.h"
#include "../balboaspa.h"

namespace esphome {
namespace balboa_spa {

class BalboaSpaNumber : public number::Number, public Component {
  public:
    void set_parent(BalboaSpa *parent) { this->parent_ = parent; }

  protected:
    BalboaSpa *parent_;
};

class SpaHourNumber : public BalboaSpaNumber {
  protected:
    void control(float value) override;
};

class SpaMinuteNumber : public BalboaSpaNumber {
  protected:
    void control(float value) override;
};

class Filter1StartHourNumber : public BalboaSpaNumber {
  protected:
    void control(float value) override;
};

class Filter1StartMinuteNumber : public BalboaSpaNumber {
  protected:
    void control(float value) override;
};

class Filter1DurationHourNumber : public BalboaSpaNumber {
  protected:
    void control(float value) override;
};

class Filter1DurationMinuteNumber : public BalboaSpaNumber {
  protected:
    void control(float value) override;
};

class Filter2StartHourNumber : public BalboaSpaNumber {
  protected:
    void control(float value) override;
};

class Filter2StartMinuteNumber : public BalboaSpaNumber {
  protected:
    void control(float value) override;
};

class Filter2DurationHourNumber : public BalboaSpaNumber {
  protected:
    void control(float value) override;
};

class Filter2DurationMinuteNumber : public BalboaSpaNumber {
  protected:
    void control(float value) override;
};

}  // namespace balboa_spa
}  // namespace esphome