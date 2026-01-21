#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "../balboaspa.h"

namespace esphome
{
    namespace balboa_spa
    {

        class Filter2Switch : public switch_::Switch
        {
        public:
            Filter2Switch() {};
            void update(SpaFilterSettings *filterSettings);
            void set_parent(BalboaSpa *parent);

        protected:
            void write_state(bool state) override;

        private:
            BalboaSpa *spa;
            static const char *TAG;
        };

    } // namespace balboa_spa
} // namespace esphome
