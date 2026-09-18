#include <stdint.h>

#ifndef SPA_STATE_H
#define SPA_STATE_H

namespace esphome {
namespace balboa_spa {

class SpaState {
    public:
        SpaState() {
            jet1 = 0;
            jet2 = 0;
            jet3 = 0;
            jet4 = 0;
            blower = 0;
            light = 0;
            light2 = 0;
            highrange = 0;
            circulation = 0;
            cleanup_cycle = 0;
            hour = 0;
            minutes = 0;
            rest_mode = 254;
            heat_state = 254;
            target_temp = NAN;
            current_temp = NAN;
            reminder = 0;
        }
        uint8_t jet1 :2;
        uint8_t jet2 :2;
        uint8_t jet3 :2;
        uint8_t jet4 :2;
        uint8_t blower :1;
        uint8_t light :1;
        uint8_t light2 :1;
        uint8_t highrange:1;        
        uint8_t circulation:1;
        uint8_t cleanup_cycle:1;
        uint8_t hour:5;
        uint8_t minutes:6;
        uint8_t rest_mode;
        uint8_t heat_state;
        float target_temp;
        float current_temp;
        uint8_t reminder;
};
}  // namespace balboa_spa
}  // namespace esphome

#endif