#include <stdint.h>

#ifndef SPA_STATE_H
#define SPA_STATE_H

namespace esphome {
namespace balboa_spa {

class SpaState {
    public:
        SpaState() {
            rest_mode = 254;
            heat_state = 254;
            target_temp = NAN;
            current_temp = NAN;
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
        uint8_t hour:5;
        uint8_t minutes:6;
        uint8_t rest_mode;
        uint8_t heat_state;
        float target_temp;
        float current_temp;
};
}  // namespace balboa_spa
}  // namespace esphome

#endif