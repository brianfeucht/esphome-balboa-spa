#include "jet2_switch.h"

namespace esphome
{
    namespace balboa_spa
    {
        void Jet2Switch::toggle_jet()
        {
            spa->toggle_jet2();
        }

    } // namespace balboa_spa
} // namespace esphome
