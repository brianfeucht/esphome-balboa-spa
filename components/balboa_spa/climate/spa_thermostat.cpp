#include "esphome.h"
#include "esphome/core/log.h"
#include "spa_thermostat.h"
#include "esphome/components/climate/climate_mode.h"

namespace esphome {
namespace balboa_spa {

static const char *TAG = "BalboaSpa.climate";

climate::ClimateTraits BalboaSpaThermostat::traits()
{
    auto traits = climate::ClimateTraits();
    traits.set_supported_modes({climate::ClimateMode::CLIMATE_MODE_OFF, climate::ClimateMode::CLIMATE_MODE_HEAT});
    traits.set_supports_action(true);
    traits.set_supports_current_temperature(true);
    traits.set_supported_presets({climate::ClimatePreset::CLIMATE_PRESET_HOME, climate::ClimatePreset::CLIMATE_PRESET_ECO});
    traits.set_supports_two_point_target_temperature(false);
    return traits;
}

 void BalboaSpaThermostat::control(const climate::ClimateCall &call){
    bool has_temp = call.get_target_temperature().has_value();
    bool has_preset = call.get_preset().has_value();
    if(has_temp){
        spa->set_temp(*call.get_target_temperature());
    }
    if (has_preset){
        spa->set_highrange( *call.get_preset() == climate::ClimatePreset::CLIMATE_PRESET_HOME );
    }
 }

void BalboaSpaThermostat::set_parent(BalboaSpa *parent) {
    spa = parent;
    parent->register_listener([this](SpaState* spaState){ this->update(spaState); });
}

bool inline is_diff_no_nan(float a, float b){
    return !std::isnan(a) && !std::isnan(b) && b != a;
}

void BalboaSpaThermostat::update(SpaState* spaState) {
    bool update = false;

    if (!spa->is_communicating()){
        this->target_temperature = NAN;
        this->current_temperature = NAN;
        return;
    }

    auto target_temp = spaState->get_target_temp();
    update = is_diff_no_nan(target_temp.value_or(NAN), this->target_temperature) || update;
    this->target_temperature = target_temp.has_value() ? target_temp.value() : this->target_temperature;

    auto ct = spaState->get_current_temp();
    update = is_diff_no_nan(ct.value_or(NAN), this->current_temperature) || update;
    this->current_temperature = ct.has_value() ? *ct : this->current_temperature; 

    auto new_action = spaState->get_heat_state() == 1 ? climate::CLIMATE_ACTION_HEATING : climate::CLIMATE_ACTION_IDLE;
    update = new_action != this->action || update;
    this->action = new_action;

    auto new_mode = spaState->get_rest_mode() == 1 ? climate::CLIMATE_MODE_OFF : climate::CLIMATE_MODE_HEAT;
    update = new_mode != this->mode || update;
    this->mode = new_mode;

    /* If highrange == 1 then the preset should be preset_home else eco */
    auto pmode = spaState->highrange == 1 ? climate::ClimatePreset::CLIMATE_PRESET_HOME : climate::ClimatePreset::CLIMATE_PRESET_ECO;
    update = pmode != this->preset || update;
    this->preset = pmode;

    update = this->lastUpdate + 300000 < millis() || update ;
    
    if(update) {
        this->publish_state();
        this->lastUpdate = millis();
    }
}

}
}