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
            // Bitfields are not zero initialised by default, and the struct is
            // read before the spa has sent its configuration message. Without
            // this constructor those first reads return whatever was on the
            // stack.
            SpaConfig()
                : pump1(0), pump2(0), pump3(0), pump4(0), pump5(0), pump6(0),
                  light1(0), light2(0), circ(0), blower(0), mister(0),
                  aux1(0), aux2(0), temperature_scale(0), clock_mode(0),
                  valid(0) {}

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
            uint8_t valid : 1;            // 1 once the spa has reported its configuration
        };

    } // namespace balboa_spa
} // namespace esphome

#endif