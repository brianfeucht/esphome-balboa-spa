#include "jet1_fan.h"

namespace esphome
{
    namespace balboa_spa
    {
        void Jet1Fan::toggle_jet()
        {
            spa->toggle_jet1();
        }

    } // namespace balboa_spa
} // namespace esphome
