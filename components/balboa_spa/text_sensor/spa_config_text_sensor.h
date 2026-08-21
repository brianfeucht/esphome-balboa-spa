#pragma once

#include "esphome/components/text_sensor/text_sensor.h"
#include "../balboaspa.h"

namespace esphome
{
    namespace balboa_spa
    {

        /**
         * @brief Exposes the hardware configuration the spa reports about itself.
         *
         * The mainboard sends a configuration message shortly after boot that
         * describes how many pumps are fitted and how many speeds each has, and
         * whether a circulation pump, blower, mister, lights or aux outputs are
         * present. That message was decoded and logged but never surfaced, so
         * there was no way to tell from Home Assistant which hardware a given
         * spa actually has.
         */
        class SpaConfigTextSensor : public text_sensor::TextSensor
        {
        public:
            void set_parent(BalboaSpa *parent);
            void update(const SpaConfig &config);

        private:
            BalboaSpa *parent_{nullptr};
            bool published_{false};
        };

    } // namespace balboa_spa
} // namespace esphome
