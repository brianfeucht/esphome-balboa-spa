#include "spa_state.h"

namespace esphome {
namespace balboa_spa {

template <typename T> uint8_t SpaValueHistory<T>::size(){ return value_history.size(); }
template <typename T> void SpaValueHistory<T>::push(T value) { value_history.push(value); }

template <typename T> bool SpaValueHistory<T>::isStable(){
     return value_history.size() > ESPHOME_BALBOASPA_MEASUREMENT_COUNT_UNTIL_STABLE;
}

template <typename T> T SpaValueHistory<T>::mode(){
    int size = value_history.size();
    T arr[size];
    value_history.copyToArray(arr);
    
    // bubble sort array
    for (int i = 0; i < size - 1; i++) {
        // Compare adjacent elements and swap them if they are in the wrong order
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    T mode = T();
    int curr_count = 1;
    int max_count = 1;
    // Iterate through the array to find the mode
    for (int i = 1; i < size; ++i) 
    {
        if (arr[i] == arr[i - 1]) {
            ++curr_count;
        }
        else {
            // Check if the current count is greater than
            // the maximum count found so far
            if (curr_count > max_count) {
                max_count = curr_count;
                mode = arr[i - 1]; // Update the mode
            }
            curr_count = 1; // Reset current count for the
                            // new element
        }
    }

    // Check if the last element forms the mode
    if (curr_count > max_count) {
        mode = arr[size - 1];
    }

    return mode;
}

float SpaState::get_current_temp(){
    if(current_temperatures.isStable() == false)
    {
        return 0;
    }
    
    return current_temperatures.mode();
}

void SpaState::set_current_temp(float current_temp){
    current_temperatures.push(current_temp);
}

float SpaState::get_target_temp(){
    if(target_temperatures.isStable() == false){
        return 0;
    }

    return target_temperatures.mode();
}

void SpaState::set_target_temp(float current_temp) {
    target_temperatures.push(current_temp);
}

uint8_t SpaState::get_heat_state(){
    if(heat_states.isStable() == false){
        return 254;
    }

    return target_temperatures.mode();
}

void SpaState::set_heat_state(uint8_t heat_state){
    heat_states.push(heat_state);
}

}}