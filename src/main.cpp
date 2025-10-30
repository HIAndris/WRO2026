#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/rmt.h"

#define TAG "ESP32S3"

#define RMT_TX_CHANNEL RMT_CHANNEL_0
#define GPIO_LED GPIO_NUM_48
#define RMT_CLK_DIV 2
#define LED_NUM 48

#define CLK_TICK_NS 25
#define T0H_NS  350 // 0 bit HIGH time (ns)
#define T0L_NS  900 // 0 bit LOW time  (ns)
#define T1H_NS  900 // 1 bit HIGH time (ns)
#define T1L_NS  350 // 1 bit LOW time  (ns)

typedef enum {
    WHITE = 0x404040,
    BLACK = 0x000000,
    RED = 0x400000,
    GREEN = 0x004000,
    BLUE = 0x000040,
    YELLOW = 0x404000,
    CYAN = 0x004040,
    MAGENTA = 0x40004,
    BRIGHT_WHITE = 0xffffff,
    DULL_WHITE = 0x101010
} led_color_t;

rmt_item32_t bit0;
rmt_item32_t bit1;

static void led_init() {
    rmt_config_t config = {};
    config.rmt_mode = RMT_MODE_TX;
    config.channel = RMT_TX_CHANNEL;
    config.gpio_num = GPIO_LED;
    config.clk_div = RMT_CLK_DIV;
    config.mem_block_num = 1;
    config.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
    config.tx_config.carrier_en = false;
    config.tx_config.loop_en = false;
    config.tx_config.idle_output_en = true;

    rmt_config(&config);
    rmt_driver_install(config.channel, 0, 0);

    bit0.level0 = 1;
    bit0.duration0 = T0H_NS / CLK_TICK_NS;
    bit0.level1 = 0;
    bit0.duration1 = T0L_NS / CLK_TICK_NS;

    bit1.level0 = 1;
    bit1.duration0 = T1H_NS / CLK_TICK_NS;
    bit1.level1 = 0;
    bit1.duration1 = T1L_NS / CLK_TICK_NS;
}

void write(const uint8_t * data, const int length) {
    rmt_item32_t items[24 * LED_NUM];
    int idx = 0;

    for (int i = 0; i < length; ++i) {
        uint8_t byte = data[i];
        for (int bit = 7; bit >= 0; --bit) {
            items[idx++] = (byte & (1 << bit)) ? bit1 : bit0;
        }
    }

    rmt_write_items(RMT_TX_CHANNEL, items, idx, true);
    rmt_wait_tx_done(RMT_TX_CHANNEL, portMAX_DELAY);
}

void set_color(const led_color_t color) {
    const auto temp = static_cast<uint32_t>(color);
    uint8_t grb[3] = {
        static_cast<uint8_t>((temp >> 8) & 0xff),
        static_cast<uint8_t>((temp >> 16) & 0xff),
        static_cast<uint8_t>(temp & 0xff)
    };

    write(grb, 3);
}

void rainbow(uint8_t brightness, uint32_t delay_ticks) {
    uint8_t grb[3] = {0x00, brightness, 0x00};
    uint8_t vi[4] = {1, 0, 2, 1};
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < brightness; ++j) {
            grb[vi[i+1]]++;
            write(grb, 3);
            ESP_LOGI(TAG, "%u %u %u", grb[1], grb[0], grb[2]);
            vTaskDelay(delay_ticks);
        }
        for (uint8_t j = 0; j < brightness; ++j) {
            grb[vi[i]]--;
            write(grb, 3);
            ESP_LOGI(TAG, "%u %u %u", grb[1], grb[0], grb[2]);
            vTaskDelay(delay_ticks);
        }
    }
}

extern "C" [[noreturn]] void app_main(void) {
    led_init();
    while (true) {
        rainbow(50, 2);
    }
    /*
    while (true) {
        for (int i = 0; i < 3; i++) {
            set_color(DULL_WHITE);
            vTaskDelay(pdMS_TO_TICKS(500));
            set_color(BLACK);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        for (int i = 0; i < 3; i++) {
            set_color(WHITE);
            vTaskDelay(pdMS_TO_TICKS(1000));
            set_color(BLACK);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        for (int i = 0; i < 3; i++) {
            set_color(BRIGHT_WHITE);
            vTaskDelay(pdMS_TO_TICKS(500));
            set_color(BLACK);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
    */
}
