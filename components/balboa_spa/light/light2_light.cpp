#include "light2_light.h"

namespace esphome
{
    namespace balboa_spa
    {
        light::LightTraits Light2Light::get_traits()
        {
            auto traits = light::LightTraits();
            traits.set_supported_color_modes({light::ColorMode::ON_OFF});
            return traits;
        }

        void Light2Light::write_state(light::LightState *state)
        {
            bool binary;
            state->current_values_as_binary(&binary);

            SpaState *spaState = spa_->get_current_state();
            if (spaState->light2 != binary)
            {
                spa_->toggle_light2();
            }
        }

        void Light2Light::set_parent(BalboaSpa *parent)
        {
            spa_ = parent;
            parent->register_listener([this](SpaState *spaState)
                                      { this->update(spaState); });
        }

        void Light2Light::update(const SpaState *spaState)
        {
            if (this->last_state_ != spaState->light2)
            {
                this->last_state_ = spaState->light2;
                if (this->state_ != nullptr)
                {
                    auto call = this->state_->make_call();
                    call.set_state(spaState->light2);
                    call.set_save(false);
                    call.perform();
                }
            }
        }

    } // namespace balboa_spa
} // namespace esphome
