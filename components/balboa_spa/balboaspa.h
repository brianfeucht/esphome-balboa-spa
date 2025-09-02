#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/log.h"

#include "spa_types.h"
#include "spa_config.h"
#include "spa_state.h"
#include "CircularBuffer.h"
#include <string>
#include <iostream>
#include <sstream>

namespace esphome
{
  namespace balboa_spa
  {

// Not defined in recent framework libs so stealing from
// https://github.com/espressif/arduino-esp32/blob/496b8411773243e1ad88a68652d6982ba2366d6b/cores/esp32/Arduino.h#L99
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)

    static const uint8_t ESPHOME_BALBOASPA_MIN_TEMPERATURE_C = 7;
    static const uint8_t ESPHOME_BALBOASPA_MAX_TEMPERATURE_C = 40;
    static const uint8_t ESPHOME_BALBOASPA_MIN_TEMPERATURE_F = 60;
    static const uint8_t ESPHOME_BALBOASPA_MAX_TEMPERATURE_F = 104;

    static const float ESPHOME_BALBOASPA_POLLING_INTERVAL = 50; // frequency to poll uart device

#define STRON "ON"
#define STROFF "OFF"

    enum TEMP_SCALE : uint8_t
    {
      UNDEFINED = 254,
      F = 0,
      C = 1
    };

    class BalboaSpa : public uart::UARTDevice, public PollingComponent
    {
    public:
      BalboaSpa() : PollingComponent(ESPHOME_BALBOASPA_POLLING_INTERVAL) {}
      void setup() override;
      void update() override;
      float get_setup_priority() const override;

      SpaConfig get_current_config();
      SpaState *get_current_state();

      void set_temp(float temp);
      void set_hour(int hour);
      void set_minute(int minute);
      void set_timescale(bool is_24h);
      void toggle_light();
      void toggle_light2();
      void toggle_jet1();
      void toggle_jet2();
      void toggle_jet3();
      void toggle_jet4();
      void toggle_blower();
      void set_highrange(bool high);

      void set_spa_temp_scale(TEMP_SCALE scale);
      void set_esphome_temp_scale(TEMP_SCALE scale);

      bool is_communicating();

      void register_listener(const std::function<void(SpaState *)> &func) { this->listeners_.push_back(func); }

      bool get_restmode();
      void toggle_heat();

    private:
      CircularBuffer<uint8_t, 100> input_queue;
      CircularBuffer<uint8_t, 100> output_queue;
      uint8_t received_byte, loop_index, temp_index;
      uint8_t last_state_crc = 0x00;
      uint8_t send_command = 0x00;
      uint8_t target_temperature = 0x00;
      uint8_t target_hour = 0x00;
      uint8_t target_minute = 0x00;
      uint8_t client_id = 0x00;
      uint32_t last_received_time = 0;
      uint8_t send_preference_code = 0;
      uint8_t send_preference_data = 0;

      TEMP_SCALE spa_temp_scale = TEMP_SCALE::UNDEFINED;
      TEMP_SCALE esphome_temp_scale = TEMP_SCALE::C;
      float convert_c_to_f(float c);
      float convert_f_to_c(float f);

      std::vector<std::function<void(SpaState *)>> listeners_;

      char config_request_status = 0;         // stages: 0-> want it; 1-> requested it; 2-> got it; 3-> further processed it
      char faultlog_request_status = 0;       // stages: 0-> want it; 1-> requested it; 2-> got it; 3-> further processed it
      char filtersettings_request_status = 0; // stages: 0-> want it; 1-> requested it; 2-> got it; 3-> further processed it
      char faultlog_update_timer = 0;         // temp logic so we only get the fault log once per 5 minutes
      char filtersettings_update_timer = 0;   // temp logic so we only get the filter settings once per 5 minutes

      SpaConfig spaConfig;
      SpaState spaState;
      SpaFaultLog spaFaultLog;
      SpaFilterSettings spaFilterSettings;

      void read_serial();
      void update_sensors();

      uint8_t crc8(CircularBuffer<uint8_t, 100> &data, bool ignore_delimiter);
      void ID_request();
      void ID_ack();
      void rs485_send();
      void print_msg(CircularBuffer<uint8_t, 100> &data);
      void decodeSettings();
      void decodeState();
      void decodeFilterSettings();
      void decodeFault();
    };

  } // namespace empty_uart_component
} // namespace esphome
