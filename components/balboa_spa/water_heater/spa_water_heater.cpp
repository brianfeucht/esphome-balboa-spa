#include "esphome.h"
#include "esphome/core/log.h"
#include "spa_water_heater.h"

namespace esphome
{
    namespace balboa_spa
    {

        static const char *TAG = "balboa_spa.water_heater";

        // Modes:
        //   ECO         → rest_mode=1 (energy-saving sleep/rest mode)
        //   HEAT_PUMP   → rest_mode=0, highrange=0 (ready, standard temp range)
        //   PERFORMANCE → rest_mode=0, highrange=1 (ready, high temp range)

        water_heater::WaterHeaterTraits BalboaSpaWaterHeater::traits()
        {
            auto traits = water_heater::WaterHeaterTraits();
            traits.set_supported_modes({
                water_heater::WATER_HEATER_MODE_ECO,
                water_heater::WATER_HEATER_MODE_HEAT_PUMP,
                water_heater::WATER_HEATER_MODE_PERFORMANCE,
            });
            traits.set_supports_current_temperature(true);
            return traits;
        }

        void BalboaSpaWaterHeater::control(const water_heater::WaterHeaterCall &call)
        {
            if (call.get_target_temperature().has_value())
            {
                spa->set_temp(*call.get_target_temperature());
            }

            if (call.get_mode().has_value())
            {
                auto requested_mode = *call.get_mode();
                bool is_in_rest = spa->get_restmode();

                if (requested_mode == water_heater::WATER_HEATER_MODE_ECO)
                {
                    // ECO = rest/sleep mode (energy saving)
                    if (!is_in_rest)
                    {
                        ESP_LOGD(TAG, "Switching to ECO (rest) mode");
                        spa->toggle_heat();
                    }
                }
                else if (requested_mode == water_heater::WATER_HEATER_MODE_HEAT_PUMP)
                {
                    // HEAT_PUMP = ready mode, standard temp range
                    spa->set_highrange(false);
                    if (is_in_rest)
                    {
                        ESP_LOGD(TAG, "Switching to HEAT_PUMP (ready, low range) mode");
                        spa->toggle_heat();
                    }
                }
                else if (requested_mode == water_heater::WATER_HEATER_MODE_PERFORMANCE)
                {
                    // PERFORMANCE = ready mode, high temp range
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
            parent->register_listener([this](SpaState *spaState)
                                      { this->update(spaState); });
        }

        bool inline wh_is_diff_no_nan(float a, float b)
        {
            return !std::isnan(a) && !std::isnan(b) && b != a;
        }

        void BalboaSpaWaterHeater::update(SpaState *spaState)
        {
            bool needs_update = false;

            if (!spa->is_communicating())
            {
                this->target_temperature = NAN;
                this->current_temperature = NAN;
                return;
            }

            float target_temp = spaState->target_temp;
            needs_update = wh_is_diff_no_nan(target_temp, this->target_temperature) || needs_update;
            this->target_temperature = !std::isnan(target_temp) ? target_temp : this->target_temperature;

            float current_temp = spaState->current_temp;
            needs_update = wh_is_diff_no_nan(current_temp, this->current_temperature) || needs_update;
            this->current_temperature = !std::isnan(current_temp) ? current_temp : this->current_temperature;

            // Map spa state to water heater mode.
            // rest_mode=254 is undefined (spa not yet initialised); skip mode update.
            if (spaState->rest_mode != 254)
            {
                water_heater::WaterHeaterMode new_mode;
                if (spaState->rest_mode == 1)
                {
                    new_mode = water_heater::WATER_HEATER_MODE_ECO;
                }
                else if (spaState->highrange == 1)
                {
                    new_mode = water_heater::WATER_HEATER_MODE_PERFORMANCE;
                }
                else
                {
                    new_mode = water_heater::WATER_HEATER_MODE_HEAT_PUMP;
                }

                needs_update = new_mode != this->mode || needs_update;
                this->mode = new_mode;
            }

            needs_update = this->last_update_time + 300000 < millis() || needs_update;

            if (needs_update)
            {
                this->publish_state();
                this->last_update_time = millis();
            }
        }

    } // namespace balboa_spa
} // namespace esphome
