#ifndef WRO26_LED_HPP
#define WRO26_LED_HPP

#include "driver/rmt.h"
#include "config/ledConfig.hpp"

namespace LED {
    class Led {
        public:
        Led();
        void set_color(COLOR color) const;
        void rainbow(uint8_t brightness, uint32_t delay_ticks) const;

        private:
        rmt_item32_t bit0;
        rmt_item32_t bit1;
        void write(const uint8_t * data, int length) const;
    };
}

#endif