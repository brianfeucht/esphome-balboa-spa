#include "spa_state.h"

namespace esphome {
namespace balboa_spa {

float SpaState::get_current_temp(){
    if(current_temp_history.isEmpty())
    {
        return 0;
    }
    
    return current_temp_history.last();
}

void SpaState::set_current_temp(float current_temp){
    current_temp_history.push(current_temp);
}

}}