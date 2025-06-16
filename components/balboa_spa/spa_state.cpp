#include "spa_state.h"

namespace esphome {
namespace balboa_spa {

template <typename T> uint8_t SpaValueHistory<T>::size(){ return value_history.size(); }
template <typename T> void SpaValueHistory<T>::push(T value) { value_history.push(value); }
template <typename T> T SpaValueHistory<T>::last() { return value_history.last(); }

template <typename T> bool SpaValueHistory<T>::isStable(){
     return value_history.size() > ESPHOME_BALBOASPA_MEASUREMENT_COUNT_UNTIL_STABLE;
}

template <typename T> T SpaValueHistory<T>::mode(){
    CircularBuffer<T, ESPHOME_BALBOASPA_MEASUREMENT_POOL_SIZE> vh = value_history;
    vh.sort();
    int size = vh.size();

    T mode = T();
    int curr_count = 1;
    int max_count = 1;
    // Iterate through the array to find the mode
    for (int i = 1; i < size; ++i) 
    {
        if (vh[i] == vh[i - 1]) {
            ++curr_count;
        }
        else {
            // Check if the current count is greater than
            // the maximum count found so far
            if (curr_count > max_count) {
                max_count = curr_count;
                mode = vh[i - 1]; // Update the mode
            }
            curr_count = 1; // Reset current count for the
                            // new element
        }
    }

    // Check if the last element forms the mode
    if (curr_count > max_count) {
        mode = vh[size - 1];
    }

    return mode;
}

optional<float> SpaState::get_current_temp(){
    if(current_temperatures.isStable() == false) {
        return esphome::optional<float>();
    }

    return esphome::optional<float>(current_temperatures.mode());
}

void SpaState::set_current_temp(float current_temp){
    current_temperatures.push(current_temp);
}

optional<float> SpaState::get_target_temp(){
    if(target_temperatures.isStable() == false){
        return esphome::optional<float>();
    }

    return esphome::optional<float>(target_temperatures.mode());
}

void SpaState::set_target_temp(float current_temp) {
    target_temperatures.push(current_temp);
}

uint8_t SpaState::get_heat_state(){
    if(heat_states.isStable() == false){
        return 254;
    }

    return heat_states.mode();
}

uint8_t SpaState::get_last_heat_state(){
    if(heat_states.size() > 0){
        return heat_states.last();
    }

    return 254;
}

void SpaState::set_heat_state(uint8_t heat_state){
    heat_states.push(heat_state);
}

uint8_t SpaState::get_rest_mode(){
    if(rest_modes.isStable() == false){
        return 254;
    }

    return rest_modes.mode();
}

uint8_t SpaState::get_last_rest_mode(){
    if(rest_modes.size() > 0){
        return rest_modes.last();
    }

    return 254;
}

void SpaState::set_rest_mode(uint8_t rest_mode){
    rest_modes.push(rest_mode);
}

SpaState::SpaState() {
    this->jet1 = 0;
    this->jet2 = 0;
    this->jet3 = 0;
    this->blower = 0;
    this->light = 0;
    this->highrange = 0;        
    this->circulation = 0;
    this->hour = 0;
    this->minutes = 0;
    this->spa_state_byte0 = 0;
    this->hold_minutes = 0;
}

}
}