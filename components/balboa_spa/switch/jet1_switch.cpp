#include "jet1_switch.h"

namespace esphome
{
    namespace balboa_spa
    {
        void Jet1Switch::toggle_jet()
        {
            spa->toggle_jet1();
        }

    } // namespace balboa_spa
} // namespace esphome
