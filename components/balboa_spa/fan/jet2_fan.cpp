#include "jet2_fan.h"

namespace esphome
{
    namespace balboa_spa
    {
        void Jet2Fan::toggle_jet()
        {
            spa->toggle_jet2();
        }

    } // namespace balboa_spa
} // namespace esphome
