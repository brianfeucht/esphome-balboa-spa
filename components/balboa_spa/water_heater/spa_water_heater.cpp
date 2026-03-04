#include "esphome.h"
#include "esphome/core/log.h"
#include "spa_water_heater.h"

namespace esphome
{
    namespace balboa_spa
    {

        static const char *TAG = "balboa_spa.water_heater";

        // Modes:
        //   OFF         → rest_mode=1 (sleep/rest, energy-saving standby)
        //   ECO         → rest_mode=0, highrange=0 (ready, standard temp range)
        //   PERFORMANCE → rest_mode=0, highrange=1 (ready, high temp range)

        water_heater::WaterHeaterTraits BalboaSpaWaterHeater::traits()
        {
            auto traits = water_heater::WaterHeaterTraits();
            traits.set_supported_modes({
                water_heater::WATER_HEATER_MODE_OFF,
                water_heater::WATER_HEATER_MODE_ECO,
                water_heater::WATER_HEATER_MODE_PERFORMANCE,
            });
            traits.set_supports_current_temperature(true);
            return traits;
        }

        void BalboaSpaWaterHeater::setup()
        {
            if (this->setup_complete_)
                return;
            this->setup_complete_ = true;

            // Ensure an initial state is published so HA can mark the entity as available.
            this->mode_ = water_heater::WATER_HEATER_MODE_OFF;
            this->publish_state();
        }

        void BalboaSpaWaterHeater::control(const water_heater::WaterHeaterCall &call)
        {
            float target_temp = call.get_target_temperature();
            if (!std::isnan(target_temp))
            {
                spa->set_temp(target_temp);
            }

            if (call.get_mode().has_value())
            {
                auto requested_mode = *call.get_mode();
                bool is_in_rest = spa->get_restmode();

                if (requested_mode == water_heater::WATER_HEATER_MODE_OFF)
                {
                    if (!is_in_rest)
                    {
                        ESP_LOGD(TAG, "Switching to OFF (rest) mode");
                        spa->toggle_heat();
                    }
                }
                else if (requested_mode == water_heater::WATER_HEATER_MODE_ECO)
                {
                    spa->set_highrange(false);
                    if (is_in_rest)
                    {
                        ESP_LOGD(TAG, "Switching to ECO (ready, standard range) mode");
                        spa->toggle_heat();
                    }
                }
                else if (requested_mode == water_heater::WATER_HEATER_MODE_PERFORMANCE)
                {
                    spa->set_highrange(true);
                    if (is_in_rest)
                    {
                        ESP_LOGD(TAG, "Switching to PERFORMANCE (ready, high range) mode");
                        spa->toggle_heat();
                    }
                }
            }
        }

        void BalboaSpaWaterHeater::set_parent(BalboaSpa *parent)
        {
            spa = parent;
            this->setup();
            parent->register_listener([this](SpaState *spaState)
                                      { this->update(spaState); });
        }

        void BalboaSpaWaterHeater::update(SpaState *spaState)
        {
            bool needs_update = false;

            if (!spa->is_communicating())
            {
                this->target_temperature_ = NAN;
                this->current_temperature_ = NAN;
                return;
            }

            float target_temp = spaState->target_temp;
            if (!std::isnan(target_temp) && target_temp != this->target_temperature_)
            {
                this->target_temperature_ = target_temp;
                needs_update = true;
            }

            float current_temp = spaState->current_temp;
            if (!std::isnan(current_temp) && current_temp != this->current_temperature_)
            {
                this->current_temperature_ = current_temp;
                needs_update = true;
            }

            // Map spa state to water heater mode.
            // rest_mode=254 is undefined (spa not yet initialised); skip update.
            if (spaState->rest_mode != 254)
            {
                water_heater::WaterHeaterMode new_mode;
                if (spaState->rest_mode == 1)
                {
                    new_mode = water_heater::WATER_HEATER_MODE_OFF;
                }
                else if (spaState->highrange == 1)
                {
                    new_mode = water_heater::WATER_HEATER_MODE_PERFORMANCE;
                }
                else
                {
                    new_mode = water_heater::WATER_HEATER_MODE_ECO;
                }

                if (new_mode != this->mode_)
                {
                    this->mode_ = new_mode;
                    needs_update = true;
                }
            }

            if (needs_update || this->last_update_time + 300000 < millis())
            {
                this->publish_state();
                this->last_update_time = millis();
            }
        }

    } // namespace balboa_spa
} // namespace esphome
