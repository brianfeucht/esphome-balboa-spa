#include "spa_config_text_sensor.h"

namespace esphome
{
    namespace balboa_spa
    {

        void SpaConfigTextSensor::set_parent(BalboaSpa *parent)
        {
            this->parent_ = parent;
            parent->register_listener([this](SpaState *spaState)
                                      { this->update(this->parent_->get_current_config()); });
        }

        void SpaConfigTextSensor::update(const SpaConfig &config)
        {
            // The configuration is static for a given spa and arrives once, a
            // little after boot. Publish it as soon as it is valid, then stop.
            if (config.valid == 0 || this->published_)
            {
                return;
            }

            char buf[220];
            snprintf(buf, sizeof(buf),
                     "{\"pump1\":%u,\"pump2\":%u,\"pump3\":%u,\"pump4\":%u,\"pump5\":%u,\"pump6\":%u,"
                     "\"light1\":%u,\"light2\":%u,\"circ\":%u,\"blower\":%u,\"mister\":%u,"
                     "\"aux1\":%u,\"aux2\":%u,\"temp_scale\":\"%s\",\"clock\":\"%s\"}",
                     (unsigned)config.pump1, (unsigned)config.pump2, (unsigned)config.pump3,
                     (unsigned)config.pump4, (unsigned)config.pump5, (unsigned)config.pump6,
                     (unsigned)config.light1, (unsigned)config.light2, (unsigned)config.circ,
                     (unsigned)config.blower, (unsigned)config.mister,
                     (unsigned)config.aux1, (unsigned)config.aux2,
                     config.temperature_scale ? "C" : "F",
                     config.clock_mode ? "24h" : "12h");

            this->publish_state(buf);
            this->published_ = true;
        }

    } // namespace balboa_spa
} // namespace esphome
