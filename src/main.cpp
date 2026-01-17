#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "led.hpp"

static auto TAG = "ESP32_S3";

extern "C" void app_main(void) {
    const LED::Led led;

    ESP_LOGI(TAG, "Hello");

    for (uint8_t i = 0; i < 3; i++) {
        led.rainbow(50, 2);
    }
    led.set_color(LED::COLOR::BLACK);
}
