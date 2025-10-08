#include "jet4_fan.h"

namespace esphome
{
    namespace balboa_spa
    {
        void Jet4Fan::toggle_jet()
        {
            spa->toggle_jet4();
        }

    } // namespace balboa_spa
} // namespace esphome
