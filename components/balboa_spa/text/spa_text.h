#pragma once

#include "esphome/core/component.h"
#include "esphome/components/text/text.h"
#include "../balboaspa.h"
#include <string>
#include <regex>

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

    class SpaFilter1ConfigText : public text::Text
    {
    public:
      SpaFilter1ConfigText() = default;
      void set_parent(BalboaSpa *parent);

    protected:
      void control(const std::string &value) override;
      
    private:
      BalboaSpa *parent_;
      bool validate_filter_config(const std::string &config_str, uint8_t &start_hour, uint8_t &start_minute, uint8_t &duration_hour, uint8_t &duration_minute);
    };

    class SpaFilter2ConfigText : public text::Text
    {
    public:
      SpaFilter2ConfigText() = default;
      void set_parent(BalboaSpa *parent);

    protected:
      void control(const std::string &value) override;
      
    private:
      BalboaSpa *parent_;
      bool validate_filter_config(const std::string &config_str, uint8_t &start_hour, uint8_t &start_minute, uint8_t &duration_hour, uint8_t &duration_minute);
    };

  } // namespace balboa_spa
} // namespace esphome