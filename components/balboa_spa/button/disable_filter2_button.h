#pragma once

#include "esphome/components/button/button.h"
#include "../balboaspa.h"

namespace esphome
{
    namespace balboa_spa
    {

        class DisableFilter2Button : public button::Button
        {
        public:
            void set_parent(BalboaSpa *parent);

        protected:
            void press_action() override;

        private:
            BalboaSpa *parent_;
        };

    } // namespace balboa_spa
} // namespace esphome