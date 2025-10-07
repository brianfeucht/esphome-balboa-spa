#include "jet3_switch.h"

namespace esphome
{
    namespace balboa_spa
    {
        void Jet3Switch::toggle_jet()
        {
            spa->toggle_jet3();
        }

    } // namespace balboa_spa
} // namespace esphome
