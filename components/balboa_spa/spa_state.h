#include <stdint.h>
#include <CircularBuffer.hpp>

#ifndef SPA_STATE_H
#define SPA_STATE_H

static const uint8_t ESPHOME_BALBOASPA_MEASUREMENT_POOL_SIZE = 10;
static const uint8_t ESPHOME_BALBOASPA_MEASUREMENT_COUNT_UNTIL_STABLE = 5;

namespace esphome {
namespace balboa_spa {

template <typename T> class SpaValueHistory{
    public:
        SpaValueHistory() {};
        uint8_t size();
        void push(T value);
        T mode();
        bool isStable();
    private:
        uint8_t measurements_to_keep = ESPHOME_BALBOASPA_MEASUREMENT_POOL_SIZE;

        CircularBuffer<T, ESPHOME_BALBOASPA_MEASUREMENT_POOL_SIZE> value_history;
};

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
        float   target_temp;

        float get_current_temp();
        void set_current_temp(float current_temp);
    private:
        SpaValueHistory<float> temperatures;
};
}  // namespace empty_uart_component
}  // namespace esphome

#endif