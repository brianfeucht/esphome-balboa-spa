#include "lights_light.h"

namespace esphome
{
    namespace balboa_spa
    {
        light::LightTraits LightsLight::get_traits()
        {
            auto traits = light::LightTraits();
            traits.set_supported_color_modes({light::ColorMode::ON_OFF});
            return traits;
        }

        void LightsLight::write_state(light::LightState *state)
        {
            bool binary;
            state->current_values_as_binary(&binary);

            SpaState *spaState = spa_->get_current_state();
            if (spaState->light != binary)
            {
                spa_->toggle_light();
            }
        }

        void LightsLight::set_parent(BalboaSpa *parent)
        {
            spa_ = parent;
            parent->register_listener([this](SpaState *spaState)
                                      { this->update(spaState); });
        }

        void LightsLight::update(const SpaState *spaState)
        {
            if (this->last_state_ != spaState->light)
            {
                this->last_state_ = spaState->light;
                if (this->state_ != nullptr)
                {
                    auto call = this->state_->make_call();
                    call.set_state(spaState->light);
                    call.set_save(false);
                    call.perform();
                }
            }
        }

    } // namespace balboa_spa
} // namespace esphome
