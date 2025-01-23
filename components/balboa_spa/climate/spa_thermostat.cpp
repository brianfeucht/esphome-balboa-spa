#include "esphome.h"
#include "esphome/core/log.h"
#include "spa_thermostat.h"
#include "esphome/components/climate/climate_mode.h"

namespace esphome {
namespace balboa_spa {

climate::ClimateTraits BalboaSpaThermostat::traits()
{
    auto traits = climate::ClimateTraits();
    traits.set_supported_modes({climate::CLIMATE_MODE_OFF, climate::ClimateMode::CLIMATE_MODE_HEAT});
    traits.set_supports_action(true);
    traits.set_supports_current_temperature(true);
    traits.set_supports_two_point_target_temperature(false);
    traits.set_visual_min_temperature(ESPHOME_BALBOASPA_MIN_TEMPERATURE);
    traits.set_visual_max_temperature(ESPHOME_BALBOASPA_MAX_TEMPERATURE);
    traits.set_visual_temperature_step(ESPHOME_BALBOASPA_TEMPERATURE_STEP);
    return traits;
}

 void BalboaSpaThermostat::control(const climate::ClimateCall &call){
    bool has_temp = call.get_target_temperature().has_value();
    if(has_temp){
        spa->set_temp(*call.get_target_temperature());
    }
 }

void BalboaSpaThermostat::set_parent(BalboaSpa *parent) { spa = parent; }

void BalboaSpaThermostat::update() {
    yield();
    SpaState spaState = spa->get_current_state();
    bool update = false;

    if(this->target_temperature != spaState.target_temp)
    {
        this->target_temperature = spaState.target_temp;
        update = true;
    }

    if(this->current_temperature != spaState.current_temp)
    {
        this->current_temperature = spaState.current_temp;
        update = true;
    }

    if(spaState.heat_state && this->action != climate::CLIMATE_ACTION_HEATING)
    {
        this->action = climate::CLIMATE_ACTION_HEATING;
        update = true;
    }
    else if(this->action != climate::CLIMATE_ACTION_IDLE)
    {
        this->action = climate::CLIMATE_ACTION_IDLE;
        update = true;
    }

    if(spaState.restmode && this->mode != climate::CLIMATE_MODE_OFF)
    {
        this->mode = climate::CLIMATE_MODE_OFF;
        update = true;
    }
    else if(this->mode != climate::CLIMATE_MODE_HEAT)
    {
        this-> mode = climate::CLIMATE_MODE_HEAT;
        update = true;
    }

    if(update)
    {
        this->publish_state();
    }
}

}
}