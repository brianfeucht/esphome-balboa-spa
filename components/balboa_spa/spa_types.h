#include "stdint.h"
#include <string>

#ifndef SPA_TYPES_H
#define SPA_TYPES_H

namespace esphome
{
    namespace balboa_spa
    {
        struct SpaFaultLog
        {
            uint8_t total_entries : 5;
            uint8_t current_entry : 5;
            uint8_t fault_code : 6;
            std::string fault_message;
            uint8_t days_ago : 8;
            uint8_t hour : 5;
            uint8_t minutes : 6;
        };

        struct SpaFilterSettings
        {
            uint8_t filter1_hour : 5;
            uint8_t filter1_minute : 6;
            uint8_t filter1_duration_hour : 5;
            uint8_t filter1_duration_minute : 6;
            uint8_t filter2_enable : 1;
            uint8_t filter2_hour : 5;
            uint8_t filter2_minute : 6;
            uint8_t filter2_duration_hour : 5;
            uint8_t filter2_duration_minute : 6;
        };

        enum ToggleStateMaybe
        {
            OFF = 0,
            ON,
            HIGH_SPEED,
            DONT_KNOW
        };

        static const char *TOGGLE_STATE_MAYBE_STRINGS[] = {
            "OFF",
            "ON",
            "HIGH",
            "DONT_KNOW"};
    } // namespace balboa_spa
} // namespace esphome

#endif