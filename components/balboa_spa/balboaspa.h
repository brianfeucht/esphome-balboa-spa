#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

#include "spa_types.h"
#include <CircularBuffer.hpp>

namespace esphome {
namespace balboa_spa {

// Not defined in recent framework libs so stealing from
// https://github.com/espressif/arduino-esp32/blob/496b8411773243e1ad88a68652d6982ba2366d6b/cores/esp32/Arduino.h#L99
#define bitRead(value, bit)            (((value) >> (bit)) & 0x01)

static const uint8_t ESPHOME_BALBOASPA_MIN_TEMPERATURE = 62; // degrees F
static const uint8_t ESPHOME_BALBOASPA_MAX_TEMPERATURE = 105; // degrees F
static const float   ESPHOME_BALBOASPA_TEMPERATURE_STEP = 1.0; // temperature setting step in degrees F


class BalboaSpa : public uart::UARTDevice, public Component {
  public:
    void setup() override;
    void loop() override;
    float get_setup_priority() const override;

    SpaConfig get_current_config();
    SpaState get_current_state();

    void set_temp(int temp);
    void set_hour(int hour);
    void set_minute(int minute);
    void toggle_light();
    void toggle_jet1() ;
    void toggle_jet2();

  private:
    CircularBuffer<uint8_t, 35> Q_in;
    CircularBuffer<uint8_t, 35> Q_out;
    uint8_t x, i, j;
    uint8_t last_state_crc = 0x00;
    uint8_t send = 0x00;
    uint8_t settemp = 0x00;
    uint8_t sethour = 0x00;
    uint8_t setminute = 0x00;
    uint8_t id = 0x00;
    unsigned long lastrx = 0;

    char have_config = 0; //stages: 0-> want it; 1-> requested it; 2-> got it; 3-> further processed it
    char have_faultlog = 0; //stages: 0-> want it; 1-> requested it; 2-> got it; 3-> further processed it
    char have_filtersettings = 0; //stages: 0-> want it; 1-> requested it; 2-> got it; 3-> further processed it
    char faultlog_minutes = 0; //temp logic so we only get the fault log once per 5 minutes
    char filtersettings_minutes = 0; //temp logic so we only get the filter settings once per 5 minutes

    SpaConfig spaConfig;
    SpaState spaState;
    SpaFaultLog spaFaultLog;
    SpaFilterSettings spaFilterSettings;

    void read_serial();
    void update_sensors();

    uint8_t crc8(CircularBuffer<uint8_t, 35> &data);
    void ID_request();
    void ID_ack();
    void rs485_send();
    void print_msg(CircularBuffer<uint8_t, 35> &data);
    void decodeSettings();
    void decodeState();
    void decodeFilterSettings();
    void decodeFault();
};


}  // namespace empty_uart_component
}  // namespace esphome