#ifndef WRO26_LEDCONFIG_HPP
#define WRO26_LEDCONFIG_HPP

#include "driver/rmt.h"

namespace LED {
    enum class COLOR : uint32_t {
        WHITE        = 0x404040,
        BLACK        = 0x000000,
        RED          = 0x400000,
        GREEN        = 0x004000,
        BLUE         = 0x000040,
        YELLOW       = 0x404000,
        CYAN         = 0x004040,
        MAGENTA      = 0x400040,
        BRIGHT_WHITE = 0xffffff,
        DULL_WHITE   = 0x101010
    };

    constexpr rmt_channel_t RMT_TX_CHANNEL = RMT_CHANNEL_0;
    constexpr gpio_num_t GPIO_LED = GPIO_NUM_48;
    constexpr uint16_t RMT_CLK_DIV = 2;
    constexpr uint16_t LED_NUM = 48;
    constexpr uint16_t CLK_TICK_NS = 25;
    constexpr uint16_t T0H_NS = 350; // 0 bit HIGH time (ns)
    constexpr uint16_t T0L_NS = 900; // 0 bit LOW time  (ns)
    constexpr uint16_t T1H_NS = 900; // 1 bit HIGH time (ns)
    constexpr uint16_t T1L_NS = 350; // 1 bit LOW time  (ns)
}

#endif