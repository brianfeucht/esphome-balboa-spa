#pragma once

#include "esphome/core/component.h"
#include "esphome/components/text/text.h"
#include "../balboaspa.h"
#include <string>

namespace esphome
{
  namespace balboa_spa
  {

    class SpaTimeText : public text::Text
    {
    public:
      SpaTimeText() = default;
      void set_parent(BalboaSpa *parent);

    protected:
      void control(const std::string &value) override;
      
    private:
      BalboaSpa *parent_;
      bool validate_time_format(const std::string &time_str, uint8_t &hour, uint8_t &minute);
    };

    class SpaFilter1StartTimeText : public text::Text
    {
    public:
      SpaFilter1StartTimeText() = default;
      void set_parent(BalboaSpa *parent);

    protected:
      void control(const std::string &value) override;
      
    private:
      BalboaSpa *parent_;
      bool validate_time_format(const std::string &time_str, uint8_t &hour, uint8_t &minute);
    };

    class SpaFilter1DurationText : public text::Text
    {
    public:
      SpaFilter1DurationText() = default;
      void set_parent(BalboaSpa *parent);

    protected:
      void control(const std::string &value) override;
      
    private:
      BalboaSpa *parent_;
      bool validate_time_format(const std::string &time_str, uint8_t &hour, uint8_t &minute);
    };

    class SpaFilter2StartTimeText : public text::Text
    {
    public:
      SpaFilter2StartTimeText() = default;
      void set_parent(BalboaSpa *parent);

    protected:
      void control(const std::string &value) override;
      
    private:
      BalboaSpa *parent_;
      bool validate_time_format(const std::string &time_str, uint8_t &hour, uint8_t &minute);
    };

    class SpaFilter2DurationText : public text::Text
    {
    public:
      SpaFilter2DurationText() = default;
      void set_parent(BalboaSpa *parent);

    protected:
      void control(const std::string &value) override;
      
    private:
      BalboaSpa *parent_;
      bool validate_time_format(const std::string &time_str, uint8_t &hour, uint8_t &minute);
    };

  } // namespace balboa_spa
} // namespace esphome