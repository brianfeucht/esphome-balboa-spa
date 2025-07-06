#include "balboaspa.h"

namespace esphome {
namespace balboa_spa {

static const char *TAG = "BalboaSpa.component";

void BalboaSpa::setup() {
    Q_in.clear();
    Q_out.clear();
}

void BalboaSpa::update() {
    uint32_t now = millis();
    if(lastrx + 10000 < now ){
      ESP_LOGW(TAG, "No new message since %d Seconds! Mark as dead!", (now - lastrx) / 1000 );
      status_set_error("No Communication with Balboa Mainboard!");
      id = 0;
    } else if (status_has_error()) {
      status_clear_error();
    }

    while (available()) {
      read_serial();
    }

    // Run through listeners
    for (const auto &listener : this->listeners_) {
      listener(&spaState);
    }
}

float BalboaSpa::get_setup_priority() const { return esphome::setup_priority::LATE; }

SpaConfig BalboaSpa::get_current_config() { return spaConfig; }
SpaState* BalboaSpa::get_current_state() { return &spaState; }

void BalboaSpa::set_temp(float temp)
{
    float target_temp = 0.0;

    if(esphome_temp_scale == TEMP_SCALE::C &&
       temp >= ESPHOME_BALBOASPA_MIN_TEMPERATURE_C &&
       temp <= ESPHOME_BALBOASPA_MAX_TEMPERATURE_C)
    {
        target_temp = temp;
    }
    else if(esphome_temp_scale == TEMP_SCALE::F &&
       temp >= ESPHOME_BALBOASPA_MIN_TEMPERATURE_F &&
       temp <= ESPHOME_BALBOASPA_MAX_TEMPERATURE_F)
    {
        target_temp = convert_f_to_c(temp);
    }
    else {
      ESP_LOGW(TAG, "set_temp(%f): is INVALID! %d", temp, esphome_temp_scale);
      return;
    }

    if(spa_temp_scale == TEMP_SCALE::C)
    {
      settemp = target_temp * 2;
    }
    else if(spa_temp_scale == TEMP_SCALE::F){
      settemp = convert_c_to_f(target_temp);
    }
    else {
      ESP_LOGW(TAG, "set_temp(%f): spa_temp_scale not set. Ignoring %d", temp, spa_temp_scale);
      return;
    }

    send = 0xff;
}

void BalboaSpa::set_highrange(bool high){
  ESP_LOGD(TAG, "highrange=%d to %d requested", spaState.highrange, high);
  if (high != spaState.highrange){
    send = 0x50;
  }
}

void BalboaSpa::set_hour(int hour) {
    if(hour >= 0 && hour <= 23) {
        sethour = hour;
        send = 0x21;
    }
}

void BalboaSpa::set_minute(int minute) {
    if(minute >= 0 && minute <= 59) {
        setminute = minute;
        send = 0x21;
    }
}

void BalboaSpa::toggle_light() {
    send = 0x11;
}

void BalboaSpa::toggle_jet1() {
    send = 0x04;
}

void BalboaSpa::toggle_jet2() {
    send = 0x05;
}

void BalboaSpa::toggle_jet3() {
    send = 0x06;
}

void BalboaSpa::toggle_blower() {
    send = 0x0C;
}

void BalboaSpa::read_serial() {
      if (!read_byte(&x)){
        return;
      }

      // Drop until SOF is seen
      if (Q_in.first() != 0x7E && x != 0x7E) {
        Q_in.clear();
        return;
      } 

      // Double SOF-marker, drop last one
      if ( Q_in.size() >= 2 && Q_in[1] == 0x7E) {
        Q_in.pop();
        return;
      }

      Q_in.push(x);

      // Complete package
      //if (x == 0x7E && Q_in[0] == 0x7E && Q_in[1] != 0x7E) {
     if (x == 0x7E && Q_in.size() > 2 && Q_in.size() >= Q_in[1] + 2) {

        if (Q_in.size() - 2 < Q_in[1]){
          ESP_LOGD(TAG, "packet_size: %d, recv_size: %d", Q_in[1], Q_in.size());
          ESP_LOGD(TAG, "%s", "Packet incomplete!");
          Q_in.clear();
          return;
        }

        auto crc = this->crc8(Q_in, true);
        auto pcrc = Q_in[Q_in[1] ];
        if ( crc != pcrc ) {
          ESP_LOGD(TAG, "CRC %d != Packet crc %d end=0x%X", crc, pcrc, Q_in[Q_in[1] + 1]);
          Q_in.clear();
          return;
        }

        // Unregistered or yet in progress
        if (id == 0) {
          ESP_LOGD(TAG, "Spa/node/id: %s", "Unregistered");
          //if (Q_in[2] == 0xFE) print_msg(Q_in);
          print_msg(Q_in);
          // FE BF 02:got new client ID
          if (Q_in[2] == 0xFE && Q_in[4] == 0x02) {
            id = Q_in[5];
            if (id > 0x2F) id = 0x2F;
            ESP_LOGD(TAG, "Spa/node/id: Got ID: %d, acknowledging", id);
            ID_ack();
            ESP_LOGD(TAG, "Spa/node/id: %d", id);
          }

          // FE BF 00:Any new clients?
          if (Q_in[2] == 0xFE && Q_in[4] == 0x00) {
            ESP_LOGD(TAG, "Spa/node/id: %s", "Requesting ID");
            ID_request();
          }
        } 
        else if (Q_in[2] == id && Q_in[4] == 0x06) 
        { // we have an ID, do clever stuff
            // id BF 06:Ready to Send
            if (send == 0x21) {
              Q_out.push(id);
              Q_out.push(0xBF);
              Q_out.push(0x21);
              Q_out.push(sethour);
              Q_out.push(setminute);
            } 
            else if (send == 0xff) {
              // 0xff marks dirty temperature for now
              Q_out.push(id);
              Q_out.push(0xBF);
              Q_out.push(0x20);
              Q_out.push(settemp);
            } 
            else if (send == 0x00) 
            {
              if (have_config == 0) { // Get configuration of the hot tub
                Q_out.push(id);
                Q_out.push(0xBF);
                Q_out.push(0x22);
                Q_out.push(0x00);
                Q_out.push(0x00);
                Q_out.push(0x01);
                ESP_LOGD(TAG, "Spa/config/status: %s", "Getting config");
                have_config = 1;
              } 
              else if (have_faultlog == 0) { // Get the fault log
                Q_out.push(id);
                Q_out.push(0xBF);
                Q_out.push(0x22);
                Q_out.push(0x20);
                Q_out.push(0xFF);
                Q_out.push(0x00);
                have_faultlog = 1;
                ESP_LOGD(TAG, "Spa/debug/have_faultlog: %s", "requesting fault log, #1");
              } 
              else if ((have_filtersettings == 0) && (have_faultlog == 2)) { // Get the filter cycles log once we have the faultlog
                Q_out.push(id);
                Q_out.push(0xBF);
                Q_out.push(0x22);
                Q_out.push(0x01);
                Q_out.push(0x00);
                Q_out.push(0x00);
                ESP_LOGD(TAG, "Spa/debug/have_filtersettings: %s", "requesting filter settings, #1");
                have_filtersettings = 1;
              } 
              else {
                // A Nothing to Send message is sent by a client immediately after a Clear to Send message if the client has no messages to send.
                Q_out.push(id);
                Q_out.push(0xBF);
                Q_out.push(0x07);
              }
            } 
            else {
              Q_out.push(id);
              Q_out.push(0xBF);
              Q_out.push(0x11);
              Q_out.push(send);
              Q_out.push(0x00);
            }

            rs485_send();
            send = 0x00;
        } else if (Q_in[2] == id && Q_in[4] == 0x2E) {
          if (last_state_crc != Q_in[Q_in[1]]) {
            decodeSettings();
          }
        } else if (Q_in[2] == id && Q_in[4] == 0x28) {
          if (last_state_crc != Q_in[Q_in[1]]) {
            decodeFault();
          }
        } else if (Q_in[2] == 0xFF && Q_in[4] == 0x13) { // FF AF 13:Status Update - Packet index offset 5
          if (last_state_crc != Q_in[Q_in[1]]) {
            decodeState();
          }
        } else if (Q_in[2] == id && Q_in[4] == 0x23) { // FF AF 23:Filter Cycle Message - Packet index offset 5
          if (last_state_crc != Q_in[Q_in[1]]) {
            ESP_LOGD(TAG, "Spa/debug/have_faultlog: %s", "decoding filter settings");
            decodeFilterSettings();
          }
        } else {
          // DEBUG for finding meaning
          //if (Q_in[2] & 0xFE || Q_in[2] == id)
          //print_msg(Q_in);
        }

        // Clean up queue
        Q_in.clear();
      }
      lastrx = millis();
  }

  uint8_t BalboaSpa::crc8(CircularBuffer<uint8_t, 100> &data, bool ignore_delimiter) {
    unsigned long crc;
    int bit;
    uint8_t length = ignore_delimiter ? data.size() - 2 : data.size() ;

    crc = 0x02;
    for ( size_t index = ignore_delimiter ; index < length ; index++ ) {
      crc ^= data[index];
      for ( bit = 0 ; bit < 8 ; bit++ ) {
        if ( (crc & 0x80) != 0 ) {
          crc <<= 1;
          crc ^= 0x7;
        }
        else {
          crc <<= 1;
        }
      }
    }
    return crc ^ 0x02;
  }

  void BalboaSpa::ID_request() {
    Q_out.push(0xFE);
    Q_out.push(0xBF);
    Q_out.push(0x01);
    Q_out.push(0x02);
    Q_out.push(0xF1);
    Q_out.push(0x73);

    rs485_send();
  }

  void BalboaSpa::ID_ack() {
    Q_out.push(id);
    Q_out.push(0xBF);
    Q_out.push(0x03);

    rs485_send();
  }

  void BalboaSpa::rs485_send() {
    // Add telegram length
    Q_out.unshift(Q_out.size() + 2);

    // Add CRC
    Q_out.push(crc8(Q_out, false));

    // Wrap telegram in SOF/EOF
    Q_out.unshift(0x7E);
    Q_out.push(0x7E);

    for (i = 0; i < Q_out.size(); i++) {
      write(Q_out[i]);
    }

    //print_msg(Q_out);

    flush();


    // DEBUG: print_msg(Q_out);
    Q_out.clear();
  }

  void BalboaSpa::print_msg(CircularBuffer<uint8_t, 100> &data) {
    std::stringstream s;
    //for (i = 0; i < (Q_in[1] + 2); i++) {
    for (i = 0; i < data.size(); i++) {
      x = Q_in[i];
      if (x < 0x0A) s << "0";
      s << std::hex << x;
      s << " ";
    }
    yield();
  }

  void BalboaSpa::decodeSettings() {
    ESP_LOGD(TAG, "Spa/config/status: Got config");
    spaConfig.pump1 = Q_in[5] & 0x03;
    spaConfig.pump2 = (Q_in[5] & 0x0C) >> 2;
    spaConfig.pump3 = (Q_in[5] & 0x30) >> 4;
    spaConfig.pump4 = (Q_in[5] & 0xC0) >> 6;
    spaConfig.pump5 = (Q_in[6] & 0x03);
    spaConfig.pump6 = (Q_in[6] & 0xC0) >> 6;
    spaConfig.light1 = (Q_in[7] & 0x03);
    spaConfig.light2 = (Q_in[7] >> 2) & 0x03;
    spaConfig.circ = ((Q_in[8] & 0x80) != 0);
    spaConfig.blower = ((Q_in[8] & 0x03) != 0);
    spaConfig.mister = ((Q_in[9] & 0x30) != 0);
    spaConfig.aux1 = ((Q_in[9] & 0x01) != 0);
    spaConfig.aux2 = ((Q_in[9] & 0x02) != 0);
    spaConfig.temp_scale = Q_in[3] & 0x01; //Read temperature scale - 0 -> Farenheit, 1-> Celcius
    ESP_LOGD(TAG, "Spa/config/pumps1: %d", spaConfig.pump1 );
    ESP_LOGD(TAG, "Spa/config/pumps2: %d", spaConfig.pump2 );
    ESP_LOGD(TAG, "Spa/config/pumps3: %d", spaConfig.pump3 );
    ESP_LOGD(TAG, "Spa/config/pumps4: %d", spaConfig.pump4 );
    ESP_LOGD(TAG, "Spa/config/pumps5: %d", spaConfig.pump5 );
    ESP_LOGD(TAG, "Spa/config/pumps6: %d", spaConfig.pump6 );
    ESP_LOGD(TAG, "Spa/config/light1: %d", spaConfig.light1);
    ESP_LOGD(TAG, "Spa/config/light2: %d", spaConfig.light2);
    ESP_LOGD(TAG, "Spa/config/circ: %d", spaConfig.circ  );
    ESP_LOGD(TAG, "Spa/config/blower: %d", spaConfig.blower);
    ESP_LOGD(TAG, "Spa/config/mister: %d", spaConfig.mister);
    ESP_LOGD(TAG, "Spa/config/aux1: %d", spaConfig.aux1  );
    ESP_LOGD(TAG, "Spa/config/aux2: %d", spaConfig.aux2  );
    ESP_LOGD(TAG, "Spa/config/temp_scale: %d", spaConfig.temp_scale);
    have_config = 2;

    if(spa_temp_scale == TEMP_SCALE::UNDEFINED){
      spa_temp_scale = static_cast<TEMP_SCALE>(spaConfig.temp_scale);
    }
  }

  void BalboaSpa::decodeState() {
    // 25:Flag Byte 20 - Set Temperature
    double temp_read = 0.0;

    if (spa_temp_scale == TEMP_SCALE::C) {
      temp_read = Q_in[25] / 2;
    } else if (spa_temp_scale == TEMP_SCALE::F) {
      temp_read = convert_f_to_c(Q_in[25]);
    }

    if(esphome_temp_scale == TEMP_SCALE::C &&
       temp_read >= ESPHOME_BALBOASPA_MIN_TEMPERATURE_C && 
       temp_read <= ESPHOME_BALBOASPA_MAX_TEMPERATURE_C)
    {
      spaState.target_temp = temp_read;
      ESP_LOGD(TAG, "Spa/temperature/target: %.2f C", temp_read);
    }
    else if (esphome_temp_scale == TEMP_SCALE::F &&
       temp_read >= ESPHOME_BALBOASPA_MIN_TEMPERATURE_F && 
       temp_read <= ESPHOME_BALBOASPA_MAX_TEMPERATURE_F)
    {
      spaState.target_temp = convert_c_to_f(temp_read);
      ESP_LOGD(TAG, "Spa/temperature/target: %.2f F", temp_read);
    }
    else {
      ESP_LOGW(TAG, "Spa/temperature/target INVALID %2.f %.2f %d %d", 
        Q_in[25], temp_read, spaConfig.temp_scale, esphome_temp_scale);
    }

    // 7:Flag Byte 2 - Actual temperature
    if (Q_in[7] != 0xFF) 
    {
      if (spa_temp_scale == TEMP_SCALE::C) {
        temp_read = Q_in[7] / 2;
      } else if (spa_temp_scale == TEMP_SCALE::F) {
        temp_read = convert_f_to_c(Q_in[7]);
      }

      if(esphome_temp_scale == TEMP_SCALE::C)
      {
        spaState.current_temp = temp_read;
        ESP_LOGD(TAG, "Spa/temperature/current: %.2f C", temp_read);
      }
      else if (esphome_temp_scale == TEMP_SCALE::F)
      {
        spaState.current_temp = convert_c_to_f(temp_read);
        ESP_LOGD(TAG, "Spa/temperature/current: %.2f F", temp_read);
      }
      else {
        ESP_LOGW(TAG, "Spa/temperature/current INVALID %2.f %.2f %d %d", 
          Q_in[7], temp_read, spaConfig.temp_scale, esphome_temp_scale);
      }
    }

    // 8:Flag Byte 3 Hour & 9:Flag Byte 4 Minute => Time

    sethour = Q_in[8];
    setminute = Q_in[9];

    if(sethour != spaState.hour || setminute != spaState.minutes)
    {
      // Do not trigger a new state for clock
      // newState = true;
      // ESP_LOGD("Spa/time/state", s.c_str());
      spaState.hour = sethour;
      spaState.minutes = setminute;
    }

    spaState.rest_mode = Q_in[10];
    
    // 15:Flags Byte 10 / Heat status, Temp Range
    spaState.heat_state = bitRead(Q_in[15], 4);

    double d = bitRead(Q_in[15], 2);
    if (d != spaState.highrange) 
    {
      ESP_LOGD(TAG, "Spa/highrange/state: %.0f", d); //LOW
      spaState.highrange = d;
    }

    // 16:Flags Byte 11
    d = bitRead(Q_in[16], 1);
    if (d != spaState.jet1) 
    {
      ESP_LOGD(TAG, "Spa/jet_1/state: %.0f", d);
      spaState.jet1 = d;
    } 

    d = bitRead(Q_in[16], 3);
    if (d != spaState.jet2) 
    {
      ESP_LOGD(TAG, "Spa/jet_2/state: %.0f", d);
      spaState.jet2 = d;
    }

    d = bitRead(Q_in[16], 5);
    if (d != spaState.jet3) 
    {
      ESP_LOGD(TAG, "Spa/jet_3/state: %.0f", d);
      spaState.jet3 = d;
    }

    // 18:Flags Byte 13
    d = bitRead(Q_in[18], 1);
    if (d != spaState.circulation)
    {
      ESP_LOGD(TAG, "Spa/circ/state: %.0f", d);
      spaState.circulation = d;
    }

    d = bitRead(Q_in[18], 2);
    if (d != spaState.blower) 
    {
      ESP_LOGD(TAG, "Spa/blower/state: %.0f", d);
      spaState.blower = d;      
    }

    d = Q_in[19] == 0x03;
    // 19:Flags Byte 14
    if (d != spaState.light) 
    {
      ESP_LOGD(TAG, "Spa/light/state: %.0f", d);
      spaState.light = d;
    }

    // TODO: callback on newState

    last_state_crc = Q_in[Q_in[1]];
  }

  void BalboaSpa::decodeFilterSettings() {
    spaFilterSettings.filt1Hour = Q_in[5];
    spaFilterSettings.filt1Minute = Q_in[6];
    spaFilterSettings.filt1DurationHour = Q_in[7];
    spaFilterSettings.filt1DurationMinute = Q_in[8];
    spaFilterSettings.filt2Enable = bitRead(Q_in[9],7); // check
    spaFilterSettings.filt2Hour = Q_in[9] ^ (spaFilterSettings.filt2Enable << 7); // check
    spaFilterSettings.filt2Minute = Q_in[10];
    spaFilterSettings.filt2DurationHour = Q_in[11];
    spaFilterSettings.filt2DurationMinute = Q_in[12];

    //Filter 1 time conversion
    static PROGMEM const char *format_string = R"({"start":"%.2i:%.2i","duration":"%.2i:%.2i"} )";
    const auto paylen = std::snprintf(nullptr, 0, format_string, spaFilterSettings.filt1Hour, spaFilterSettings.filt1Minute, spaFilterSettings.filt1DurationHour, spaFilterSettings.filt1DurationMinute);
 
    char payload[paylen+1] = {0};
    std::snprintf(payload, paylen, format_string, spaFilterSettings.filt1Hour, spaFilterSettings.filt1Minute, spaFilterSettings.filt1DurationHour, spaFilterSettings.filt1DurationMinute);
    ESP_LOGD(TAG, "Spa/filter1/state: %s", payload);

    //Filter 2 time conversion
    ESP_LOGD(TAG, "Spa/filter2_enabled/state: %s", spaFilterSettings.filt2Enable == 1 ? STRON : STROFF);
    std::snprintf(payload, paylen, format_string, spaFilterSettings.filt2Hour, spaFilterSettings.filt2Minute, spaFilterSettings.filt2DurationHour, spaFilterSettings.filt2DurationMinute);
    ESP_LOGD(TAG, "Spa/filter2/state: %s", payload);

    have_filtersettings = 2;
  }

  void BalboaSpa::decodeFault() {
    spaFaultLog.totEntry = Q_in[5];
    spaFaultLog.currEntry = Q_in[6];
    spaFaultLog.faultCode = Q_in[7];
    switch (spaFaultLog.faultCode) { // this is a inelegant way to do it, a lookup table would be better
      case 15:
        spaFaultLog.faultMessage = "Sensors are out of sync";
        break;
      case 16:
        spaFaultLog.faultMessage = "The water flow is low";
        break;
      case 17:
        spaFaultLog.faultMessage = "The water flow has failed";
        break;
      case 18:
        spaFaultLog.faultMessage = "The settings have been reset";
        break;
      case 19:
        spaFaultLog.faultMessage = "Priming Mode";
        break;
      case 20:
        spaFaultLog.faultMessage = "The clock has failed";
        break;
      case 21:
        spaFaultLog.faultMessage = "The settings have been reset";
        break;
      case 22:
        spaFaultLog.faultMessage = "Program memory failure";
        break;
      case 26:
        spaFaultLog.faultMessage = "Sensors are out of sync -- Call for service";
        break;
      case 27:
        spaFaultLog.faultMessage = "The heater is dry";
        break;
      case 28:
        spaFaultLog.faultMessage = "The heater may be dry";
        break;
      case 29:
        spaFaultLog.faultMessage = "The water is too hot";
        break;
      case 30:
        spaFaultLog.faultMessage = "The heater is too hot";
        break;
      case 31:
        spaFaultLog.faultMessage = "Sensor A Fault";
        break;
      case 32:
        spaFaultLog.faultMessage = "Sensor B Fault";
        break;
      case 34:
        spaFaultLog.faultMessage = "A pump may be stuck on";
        break;
      case 35:
        spaFaultLog.faultMessage = "Hot fault";
        break;
      case 36:
        spaFaultLog.faultMessage = "The GFCI test failed";
        break;
      case 37:
        spaFaultLog.faultMessage = "Standby Mode (Hold Mode)";
        break;
      default:
        spaFaultLog.faultMessage = "Unknown error";
        break;
    }
    spaFaultLog.daysAgo = Q_in[8];
    spaFaultLog.hour = Q_in[9];
    spaFaultLog.minutes = Q_in[10];
    ESP_LOGD(TAG, "Spa/fault/Entries: %d", spaFaultLog.totEntry);
    ESP_LOGD(TAG, "Spa/fault/Entry: %d", spaFaultLog.currEntry);
    ESP_LOGD(TAG, "Spa/fault/Code: %d", spaFaultLog.faultCode);
    ESP_LOGD(TAG, "Spa/fault/Message: %s", spaFaultLog.faultMessage.c_str());
    ESP_LOGD(TAG, "Spa/fault/DaysAgo: %d", spaFaultLog.daysAgo);
    ESP_LOGD(TAG, "Spa/fault/Hours: %d", spaFaultLog.hour);
    ESP_LOGD(TAG, "Spa/fault/Minutes: %d", spaFaultLog.minutes);
    have_faultlog = 2;
    //ESP_LOGD(TAG, "Spa/debug/have_faultlog: have the faultlog, #2");
  }

  bool BalboaSpa::is_communicating(){
    return id != 0;
  }

  void BalboaSpa::set_spa_temp_scale(TEMP_SCALE scale) {
    spa_temp_scale = scale;
  }

  void BalboaSpa::set_esphome_temp_scale(TEMP_SCALE scale) {
    esphome_temp_scale = scale;
  }

  float BalboaSpa::convert_c_to_f(float c) {
    return (c * 9.0/5.0) + 32.0;
  }

  float BalboaSpa::convert_f_to_c(float f) {
    return (f - 32.0) * 5.0/9.0;
  }
}  // namespace balboa_spa
}  // namespace esphome
