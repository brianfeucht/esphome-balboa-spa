#include <stdint.h>
#include <CircularBuffer.hpp>

#ifndef SPA_STATE_H
#define SPA_STATE_H

static const uint8_t ESPHOME_BALBOASPA_MEASUREMENT_POOL_SIZE = 20;
static const uint8_t ESPHOME_BALBOASPA_MEASUREMENT_COUNT_UNTIL_STABLE = 5;

namespace esphome {
namespace balboa_spa {

template <typename T> class SpaValueHistory{
    public:
        SpaValueHistory() {};
        uint8_t size();
        void push(T value);
        T mode();
        T last();
        bool isStable();

        uint8_t measurements_to_keep = ESPHOME_BALBOASPA_MEASUREMENT_POOL_SIZE;
        uint8_t stable_threshold = ESPHOME_BALBOASPA_MEASUREMENT_COUNT_UNTIL_STABLE;
    private:
        CircularBuffer<T, ESPHOME_BALBOASPA_MEASUREMENT_POOL_SIZE> value_history;
};

class SpaState {
    public:
        SpaState() {}
        uint8_t jet1 :2;
        uint8_t jet2 :2;
        uint8_t blower :1;
        uint8_t light :1;
        uint8_t highrange:1;
        uint8_t circulation:1;
        uint8_t hour :5;
        uint8_t minutes :6;

        uint8_t get_rest_mode();
        uint8_t get_last_rest_mode();
        void set_rest_mode(uint8_t restmode);

        uint8_t get_heat_state();
        uint8_t get_last_heat_state();
        void set_heat_state(uint8_t heat_state);

        float get_target_temp();
        void set_target_temp(float target_temp);

        float get_current_temp();
        void set_current_temp(float current_temp);

        void set_state_pool_size(uint8_t pool_size);
        void set_state_pool_stable_threshold(uint8_t stable_threshold);
    private:
        SpaValueHistory<float> current_temperatures;
        SpaValueHistory<float> target_temperatures;
        SpaValueHistory<uint8_t> heat_states;
        SpaValueHistory<uint8_t> rest_modes;
};
}  // namespace empty_uart_component
}  // namespace esphome

#endif