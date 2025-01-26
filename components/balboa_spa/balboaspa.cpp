#include "esphome.h"
#include "esphome/core/log.h"
#include "balboaspa.h"

namespace esphome {
namespace balboa_spa {

static const char *TAG = "BalboaSpa.component";

void BalboaSpa::setup() {
    Q_in.clear();
    Q_out.clear();
}

void BalboaSpa::update() {
    yield();
    while (available()) {
      read_serial();
    }
}

float BalboaSpa::get_setup_priority() const { return esphome::setup_priority::LATE; }

SpaConfig BalboaSpa::get_current_config() { return spaConfig; }
SpaState BalboaSpa::get_current_state() { return spaState; }

void BalboaSpa::set_temp(float temp)
{
    if(spaConfig.temp_scale == 1){
      temp = ((temp * 9.0) / 5.0) + 32;
    }

    if (temp >= ESPHOME_BALBOASPA_MIN_TEMPERATURE || temp <= ESPHOME_BALBOASPA_MAX_TEMPERATURE) {
      settemp = temp;
      send = 0xff;
    }
}

void BalboaSpa::set_hour(int hour) {
    if(hour >= 0 || hour <= 23) {
        sethour = hour;
        send = 0x21;
    }
}

void BalboaSpa::set_minute(int minute) {
    if(minute >= 0 || minute <= 59) {
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

void BalboaSpa::read_serial() {
      x = read();
      Q_in.push(x);
      
      // Drop until SOF is seen
      if (Q_in.first() != 0x7E) {
        Q_in.clear();
      } 
        
      // Double SOF-marker, drop last one
      if (Q_in[1] == 0x7E && Q_in.size() > 1) Q_in.pop();

      // Complete package
      //if (x == 0x7E && Q_in[0] == 0x7E && Q_in[1] != 0x7E) {
      if (x == 0x7E && Q_in.size() > 2) {

        // Unregistered or yet in progress
        if (id == 0) {
          ESP_LOGD("Spa/node/id", "Unregistered");
          //if (Q_in[2] == 0xFE) print_msg(Q_in);
          print_msg(Q_in);
          // FE BF 02:got new client ID
          if (Q_in[2] == 0xFE && Q_in[4] == 0x02) {
            id = Q_in[5];
            if (id > 0x2F) id = 0x2F;
            ESP_LOGD("Spa/node/id", "Got ID, acknowledging");
            ID_ack();
            ESP_LOGD("Spa/node/id", String(id).c_str());
          }

          // FE BF 00:Any new clients?
          if (Q_in[2] == 0xFE && Q_in[4] == 0x00) {
            ESP_LOGD("Spa/node/id", "Requesting ID");
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
            else if (send == 0x00) {
              if (have_config == 0) { // Get configuration of the hot tub
                Q_out.push(id);
                Q_out.push(0xBF);
                Q_out.push(0x22);
                Q_out.push(0x00);
                Q_out.push(0x00);
                Q_out.push(0x01);
                ESP_LOGD("Spa/config/status", "Getting config");
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
                ESP_LOGD("Spa/debug/have_faultlog", "requesting fault log, #1");
              } 
              else if ((have_filtersettings == 0) && (have_faultlog == 2)) { // Get the filter cycles log once we have the faultlog
                Q_out.push(id);
                Q_out.push(0xBF);
                Q_out.push(0x22);
                Q_out.push(0x01);
                Q_out.push(0x00);
                Q_out.push(0x00);
                ESP_LOGD("Spa/debug/have_filtersettings", "requesting filter settings, #1");
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
              // Send toggle commands
              ESP_LOGD("Spa/debug/SEND/id",String(id).c_str());
              ESP_LOGD("Spa/debug/SEND/send",String(send).c_str());
              /*
              Q_out.push(id);
              Q_out.push(0xBF);
              Q_out.push(0x11);
              Q_out.push(send);
              Q_out.push(0x00);
              */
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
            ESP_LOGD("Spa/debug/have_faultlog", "decoding filter settings");
            decodeFilterSettings();
          }
        } else {
          // DEBUG for finding meaning
          //if (Q_in[2] & 0xFE || Q_in[2] == id)
          //print_msg(Q_in);
        }

        // Clean up queue
        yield();
        Q_in.clear();
      }
      lastrx = millis();
  }


  uint8_t BalboaSpa::crc8(CircularBuffer<uint8_t, 35> &data) {
    unsigned long crc;
    int i, bit;
    uint8_t length = data.size();

    crc = 0x02;
    for ( i = 0 ; i < length ; i++ ) {
      crc ^= data[i];
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
    Q_out.push(crc8(Q_out));

    // Wrap telegram in SOF/EOF
    Q_out.unshift(0x7E);
    Q_out.push(0x7E);

    for (i = 0; i < Q_out.size(); i++) {
      write(Q_out[i]);
    }

    print_msg(Q_out);

    flush();


    // DEBUG: print_msg(Q_out);
    Q_out.clear();
  }

  void BalboaSpa::print_msg(CircularBuffer<uint8_t, 35> &data) {
    String s;
    //for (i = 0; i < (Q_in[1] + 2); i++) {
    for (i = 0; i < data.size(); i++) {
      x = Q_in[i];
      if (x < 0x0A) s += "0";
      s += String(x, HEX);
      s += " ";
    }
    yield();
  }

  void BalboaSpa::decodeSettings() {
    ESP_LOGD("Spa/config/status", "Got config");
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
    ESP_LOGD("Spa/config/pumps1", String(spaConfig.pump1).c_str());
    ESP_LOGD("Spa/config/pumps2", String(spaConfig.pump2).c_str());
    ESP_LOGD("Spa/config/pumps3", String(spaConfig.pump3).c_str());
    ESP_LOGD("Spa/config/pumps4", String(spaConfig.pump4).c_str());
    ESP_LOGD("Spa/config/pumps5", String(spaConfig.pump5).c_str());
    ESP_LOGD("Spa/config/pumps6", String(spaConfig.pump6).c_str());
    ESP_LOGD("Spa/config/light1", String(spaConfig.light1).c_str());
    ESP_LOGD("Spa/config/light2", String(spaConfig.light2).c_str());
    ESP_LOGD("Spa/config/circ", String(spaConfig.circ).c_str());
    ESP_LOGD("Spa/config/blower", String(spaConfig.blower).c_str());
    ESP_LOGD("Spa/config/mister", String(spaConfig.mister).c_str());
    ESP_LOGD("Spa/config/aux1", String(spaConfig.aux1).c_str());
    ESP_LOGD("Spa/config/aux2", String(spaConfig.aux2).c_str());
    ESP_LOGD("Spa/config/temp_scale", String(spaConfig.temp_scale).c_str());
    have_config = 2;
  }

  void BalboaSpa::decodeState() {
    String s;
    double d = 0.0;
    double c = 0.0;
    bool newState = false;

    // 25:Flag Byte 20 - Set Temperature
    if (spaConfig.temp_scale == 0) {
      d = Q_in[25];
    } else if (spaConfig.temp_scale == 1){
      d = (Q_in[25] - 32.0) * 5.0/9.0;
    }

    if(d != spaState.target_temp)
    {
      newState = true;
      spaState.target_temp = d;
      ESP_LOGD("Spa/target_temp/state", String(d, 2).c_str());
    }

    // 7:Flag Byte 2 - Actual temperature
    if (Q_in[7] != 0xFF) 
    {
      if (spaConfig.temp_scale == 0) {
        d = Q_in[7];
      } else if (spaConfig.temp_scale == 1){
        d = (Q_in[7] - 32.0) * 5.0/9.0;
      }

      if (c > 0) {
        if ((d > c * 1.2) || (d < c * 0.8)) d = c; //remove spurious readings greater or less than 20% away from previous read
      }

      c = d;
    } else {
      d = 0;
    }

    if(d != spaState.current_temp)
    {
      newState = true;
      spaState.current_temp = d;      
      ESP_LOGD("Spa/temperature/state", String(d, 2).c_str());
    }

    // REMARK Move upper publish to HERE to get 0 for unknown temperature

    // 8:Flag Byte 3 Hour & 9:Flag Byte 4 Minute => Time
    if (Q_in[8] < 10) s = "0"; else s = "";
    sethour = Q_in[8];
    s += String(Q_in[8]) + ":";
    if (Q_in[9] < 10) s += "0";
    s += String(Q_in[9]);
    setminute = Q_in[9];

    if(sethour != spaState.hour || setminute != spaState.minutes)
    {
      // Do not trigger a new state for clock
      // newState = true;
      // ESP_LOGD("Spa/time/state", s.c_str());
      spaState.hour = sethour;
      spaState.minutes = setminute;
    }

    d = Q_in[10];
    if(d != spaState.restmode)
    {
      newState = true;
      spaState.restmode =d ;      
      ESP_LOGD("Spa/restmode/state", String(d, 0).c_str());
    }
    
    // 15:Flags Byte 10 / Heat status, Temp Range
    d = bitRead(Q_in[15], 4);    
    if (d != spaState.heat_state)
    {
      newState = true;
      ESP_LOGD("Spa/heatstate/state", String(d, 0).c_str());
      spaState.heat_state = d;
    }

    d = bitRead(Q_in[15], 2);
    if (d != spaState.highrange) 
    {
      newState = true;
      ESP_LOGD("Spa/highrange/state", String(d, 0).c_str()); //LOW
      spaState.highrange = d;
    }

    // 16:Flags Byte 11
    d = bitRead(Q_in[16], 1);
    if (d != spaState.jet1) 
    {
      newState = true;
      ESP_LOGD("Spa/jet_1/state", String(d, 0).c_str());
      spaState.jet1 = d;
    } 

    d = bitRead(Q_in[16], 3);
    if (d != spaState.jet2) 
    {
      newState = true;
      ESP_LOGD("Spa/jet_2/state", String(d, 0).c_str());
      spaState.jet2 = d;
    }

    // 18:Flags Byte 13
    d = bitRead(Q_in[18], 1);
    if (d != spaState.circulation)
    {
      newState = true;
      ESP_LOGD("Spa/circ/state", String(d, 0).c_str());
      spaState.circulation = d;
    }

    d = bitRead(Q_in[18], 2);
    if (d != spaState.blower) 
    {
      newState = true;
      ESP_LOGD("Spa/blower/state", String(d, 0).c_str());
      spaState.blower = d;      
    }

    d = Q_in[19] == 0x03;
    // 19:Flags Byte 14
    if (d != spaState.light) 
    {
      newState = true;
      ESP_LOGD("Spa/light/state",  String(d, 0).c_str());
      spaState.light = d;
    }

    // TODO: callback on newState

    last_state_crc = Q_in[Q_in[1]];
  }

  void BalboaSpa::decodeFilterSettings() {
    String s;
    String d;
    String payld;

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
    if (spaFilterSettings.filt1Hour < 10) s = "0"; else s = "";
    s = String(spaFilterSettings.filt1Hour) + ":";
    if (spaFilterSettings.filt1Minute < 10) s += "0";
    s += String(spaFilterSettings.filt1Minute);

    if (spaFilterSettings.filt1DurationHour < 10) d = "0"; else d = "";
    d = String(spaFilterSettings.filt1DurationHour) + ":";
    if (spaFilterSettings.filt1DurationMinute < 10) d += "0";
    d += String(spaFilterSettings.filt1DurationMinute);

    payld = "{\"start\":\""+s+"\",\"duration\":\""+d+"\"}";
    ESP_LOGD("Spa/filter1/state", payld.c_str());

    //Filter 2 time conversion
    if (spaFilterSettings.filt2Hour < 10) s = "0"; else s = "";
    s += String(spaFilterSettings.filt2Hour) + ":";
    if (spaFilterSettings.filt2Minute < 10) s += "0";
    s += String(spaFilterSettings.filt2Minute);

    if (spaFilterSettings.filt2DurationHour < 10) d = "0"; else d = "";
    d += String(spaFilterSettings.filt2DurationHour) + ":";
    if (spaFilterSettings.filt2DurationMinute < 10) d += "0";
    d += String(spaFilterSettings.filt2DurationMinute);
    if ((int)(spaFilterSettings.filt2Enable) == 1) ESP_LOGD("Spa/filter2_enabled/state", STRON); else ESP_LOGD("Spa/filter2_enabled/state", STROFF);


    payld = "{\"start\":\""+s+"\",\"duration\":\""+d+"\"}";
    ESP_LOGD("Spa/filter2/state", payld.c_str());

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
    ESP_LOGD("Spa/fault/Entries", String(spaFaultLog.totEntry).c_str());
    ESP_LOGD("Spa/fault/Entry", String(spaFaultLog.currEntry).c_str());
    ESP_LOGD("Spa/fault/Code", String(spaFaultLog.faultCode).c_str());
    ESP_LOGD("Spa/fault/Message", spaFaultLog.faultMessage.c_str());
    ESP_LOGD("Spa/fault/DaysAgo", String(spaFaultLog.daysAgo).c_str());
    ESP_LOGD("Spa/fault/Hours", String(spaFaultLog.hour).c_str());
    ESP_LOGD("Spa/fault/Minutes", String(spaFaultLog.minutes).c_str());
    have_faultlog = 2;
    //ESP_LOGD("Spa/debug/have_faultlog", "have the faultlog, #2");
  }

}  // namespace balboa_spa
}  // namespace esphome
