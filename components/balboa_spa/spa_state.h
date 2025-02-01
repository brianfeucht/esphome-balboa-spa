#include <stdint.h>
#include <CircularBuffer.hpp>

#ifndef SPA_STATE_H
#define SPA_STATE_H

namespace esphome {
namespace balboa_spa {
class SpaState {
    public:
        SpaState() {}
        uint8_t jet1 :2;
        uint8_t jet2 :2;
        uint8_t blower :1;
        uint8_t light :1;
        uint8_t restmode:1;
        uint8_t heat_state:2;
        uint8_t highrange:1;        
        uint8_t circulation:1;
        uint8_t hour :5;
        uint8_t minutes :6;
        float   current_temp;
        float   target_temp;

        float get_current_temp();
        void set_current_temp(float current_temp);
    private:
        CircularBuffer<uint8_t, 10> current_temp_history;

    };
}  // namespace empty_uart_component
}  // namespace esphome

#endif