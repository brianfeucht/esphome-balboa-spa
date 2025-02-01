#include "stdint.h"
#include <string>

#ifndef SPA_TYPES_H
#define SPA_TYPES_H

namespace esphome {
namespace balboa_spa {
    struct SpaState {
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
    };

    struct SpaFaultLog {
        uint8_t totEntry :5;
        uint8_t currEntry :5;
        uint8_t faultCode :6;
        std::string faultMessage;
        uint8_t daysAgo :8;
        uint8_t hour :5;
        uint8_t minutes :6;
    };

    struct SpaFilterSettings {
        uint8_t filt1Hour :5;
        uint8_t filt1Minute :6;
        uint8_t filt1DurationHour :5;
        uint8_t filt1DurationMinute :6;
        uint8_t filt2Enable :1;
        uint8_t filt2Hour :5;
        uint8_t filt2Minute :6;
        uint8_t filt2DurationHour :5;
        uint8_t filt2DurationMinute :6;

    };
}  // namespace empty_uart_component
}  // namespace esphome

#endif