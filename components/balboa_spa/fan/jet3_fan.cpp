#include "jet3_fan.h"

namespace esphome
{
    namespace balboa_spa
    {
        void Jet3Fan::toggle_jet()
        {
            spa->toggle_jet3();
        }

    } // namespace balboa_spa
} // namespace esphome
