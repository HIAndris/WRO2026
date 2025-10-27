#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_log.h"

#define TAG "ESP32S3"
#define DELAY pdMS_TO_TICKS(20)

#define BUZZER_GPIO               21
#define BUZZER_TIMER              LEDC_TIMER_0
#define BUZZER_MODE               LEDC_LOW_SPEED_MODE
#define BUZZER_DUTY               6000
#define BUZZER_START_FREQUENCY    8000

static void buzzer_init() {
    static const ledc_timer_config_t buzzer_timer = {
        .speed_mode       = BUZZER_MODE,
        .duty_resolution  = LEDC_TIMER_13_BIT,
        .timer_num        = BUZZER_TIMER,
        .freq_hz          = BUZZER_START_FREQUENCY,
        .clk_cfg          = LEDC_AUTO_CLK,
        .deconfigure      = false
    };
    ESP_ERROR_CHECK(ledc_timer_config(&buzzer_timer));

    static const ledc_channel_config_t buzzer_channel = {
        .gpio_num       = BUZZER_GPIO,
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = LEDC_TIMER_0,
        .duty           = BUZZER_DUTY,
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&buzzer_channel));
}

extern "C" [[noreturn]] void app_main(void) {
    buzzer_init();
    for (uint32_t freq = 20; freq < 10000; freq += 5) {
        ledc_set_freq(BUZZER_MODE, BUZZER_TIMER, freq);
        ESP_LOGI(TAG, "Playing sound on GPIO %d...", BUZZER_GPIO);
        vTaskDelay(DELAY);
    }
}
