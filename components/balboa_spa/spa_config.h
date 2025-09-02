#include <stdint.h>
#include <string>

#ifndef SPA_CONFIG_H
#define SPA_CONFIG_H

namespace esphome
{
    namespace balboa_spa
    {
        struct SpaConfig
        {
        public:
            uint8_t pump1 : 2; // this could be 1=1 speed; 2=2 speeds
            uint8_t pump2 : 2;
            uint8_t pump3 : 2;
            uint8_t pump4 : 2;
            uint8_t pump5 : 2;
            uint8_t pump6 : 2;
            uint8_t light1 : 1;
            uint8_t light2 : 1;
            uint8_t circ : 1;
            uint8_t blower : 1;
            uint8_t mister : 1;
            uint8_t aux1 : 1;
            uint8_t aux2 : 1;
            uint8_t temperature_scale : 1; // 1 -> Farenheit, 0-> Celcius
            uint8_t clock_mode : 1;        // 0 -> 12h, 1-> 24h
        };

    } // namespace balboa_spa
} // namespace esphome

#endif