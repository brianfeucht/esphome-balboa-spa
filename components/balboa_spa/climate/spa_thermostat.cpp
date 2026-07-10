#include "esphome.h"
#include "esphome/core/log.h"
#include "spa_thermostat.h"
#include "esphome/components/climate/climate_mode.h"
#include <cmath>

namespace esphome
{
    namespace balboa_spa
    {

        climate::ClimateTraits BalboaSpaThermostat::traits()
        {
            auto traits = climate::ClimateTraits();
            traits.set_supported_modes({climate::CLIMATE_MODE_OFF, climate::ClimateMode::CLIMATE_MODE_HEAT});
            traits.add_feature_flags(climate::CLIMATE_SUPPORTS_ACTION | climate::CLIMATE_SUPPORTS_CURRENT_TEMPERATURE);
            traits.set_supported_presets({climate::ClimatePreset::CLIMATE_PRESET_HOME, climate::ClimatePreset::CLIMATE_PRESET_ECO});
            return traits;
        }

        void BalboaSpaThermostat::control(const climate::ClimateCall &call)
        {
            bool has_temp = call.get_target_temperature().has_value();
            bool has_preset = call.get_preset().has_value();
            if (has_temp)
            {
                spa->set_temp(*call.get_target_temperature());
            }
            if (has_preset)
            {
                spa->set_highrange(*call.get_preset() == climate::ClimatePreset::CLIMATE_PRESET_HOME);
            }

            if (call.get_mode().has_value())
            {
                auto requested_mode = *call.get_mode();

                // CLIMATE_MODE_HEAT = (Ready)
                // CLIMATE_MODE_OFF = (Rest)
                // SpaState.rest_mode 1 = Rest, 0 = Ready

                bool is_in_rest = spa->get_restmode();

                if (requested_mode == climate::CLIMATE_MODE_HEAT && is_in_rest)
                {
                    ESP_LOGD("spa_thermostat", "Toggle from Rest to Heat (Ready)");
                    spa->toggle_heat();
                }
                else if (requested_mode == climate::CLIMATE_MODE_OFF && !is_in_rest)
                {
                    ESP_LOGD("spa_thermostat", "Toggle from Heat to Rest");
                    spa->toggle_heat();
                }
            }
        }

        void BalboaSpaThermostat::set_parent(BalboaSpa *parent)
        {
            spa = parent;
            parent->register_listener([this](SpaState *spaState)
                                      { this->update(spaState); });
        }

        bool inline is_diff_no_nan(float a, float b)
        {
            return !std::isnan(a) && !std::isnan(b) && b != a;
        }

        void BalboaSpaThermostat::update(SpaState *spaState)
        {
            bool needs_update = false;

            if (!spa->is_communicating())
            {
                this->target_temperature = NAN;
                this->current_temperature = NAN;
                this->pending_current_temp = NAN;
                return;
            }

            // Target Temperature
            float target_temp = spaState->target_temp;
            if (!std::isnan(target_temp))
            {
                needs_update = is_diff_no_nan(target_temp, this->target_temperature) || needs_update;
                this->target_temperature = target_temp;
            }

            // Current Temperature with Relative Filtering (scale-aware)
            float raw_current_temp = spaState->current_temp;
            if (!std::isnan(raw_current_temp))
            {
                if (std::isnan(this->current_temperature))
                {
                    // First valid reading after boot/reconnect, accept immediately
                    needs_update = is_diff_no_nan(raw_current_temp, this->current_temperature) || needs_update;
                    this->current_temperature = raw_current_temp;
                }
                else
                {
                    float delta = std::abs(raw_current_temp - this->current_temperature);
                    // Determine threshold based on configured temperature scale
                    bool is_fahrenheit = (spa->get_esphome_temp_scale() == TEMP_SCALE::F);
                    float threshold = is_fahrenheit ? 9.0f : 5.0f;  // 9°F is equivalent to 5°C
                    float tolerance = is_fahrenheit ? 1.0f : 0.5f;

                    if (delta > threshold)
                    {
                        // Check if this is a new anomaly or an ongoing one
                        if (std::isnan(this->pending_current_temp) || std::abs(raw_current_temp - this->pending_current_temp) > tolerance)
                        {
                            // Start tracking the new anomalous value
                            this->pending_current_temp = raw_current_temp;
                            this->pending_temp_start = millis();
                            ESP_LOGD("spa_thermostat", "Anomalous temp jump detected (%.1f). Waiting to stabilize.", raw_current_temp);
                        }
                        else if (millis() - this->pending_temp_start > 60000) // Settle time: 60 seconds
                        {
                            // Temp has held at this anomalous level long enough, assume water change
                            ESP_LOGD("spa_thermostat", "Anomalous temp (%.1f) stabilized. Accepting as new baseline.", raw_current_temp);
                            needs_update = is_diff_no_nan(raw_current_temp, this->current_temperature) || needs_update;
                            this->current_temperature = raw_current_temp;
                            this->pending_current_temp = NAN; // Reset tracker
                        }
                    }
                    else
                    {
                        // Normal incremental change
                        needs_update = is_diff_no_nan(raw_current_temp, this->current_temperature) || needs_update;
                        this->current_temperature = raw_current_temp;
                        this->pending_current_temp = NAN; // Reset tracker
                    }
                }
            }

            auto new_action = spaState->heat_state == 1 ? climate::CLIMATE_ACTION_HEATING : climate::CLIMATE_ACTION_IDLE;
            needs_update = new_action != this->action || needs_update;
            this->action = new_action;

            auto new_mode = spaState->rest_mode == 1 ? climate::CLIMATE_MODE_OFF : climate::CLIMATE_MODE_HEAT;
            needs_update = new_mode != this->mode || needs_update;
            this->mode = new_mode;

            /* If highrange == 1 then the preset should be preset_home else eco */
            auto preset_mode = spaState->highrange == 1 ? climate::ClimatePreset::CLIMATE_PRESET_HOME : climate::ClimatePreset::CLIMATE_PRESET_ECO;
            needs_update = preset_mode != this->preset || needs_update;
            this->preset = preset_mode;

            needs_update = this->last_update_time + 300000 < millis() || needs_update;

            if (needs_update)
            {
                this->publish_state();
                this->last_update_time = millis();
            }
        }

    }
}
