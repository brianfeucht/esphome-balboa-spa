#include "jet4_switch.h"

namespace esphome
{
    namespace balboa_spa
    {
        void Jet4Switch::toggle_jet()
        {
            spa->toggle_jet4();
        }

    } // namespace balboa_spa
} // namespace esphome
