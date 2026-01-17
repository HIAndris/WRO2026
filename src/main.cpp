#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/rmt.h"

#include "led.hpp"

#define TAG "ESP32S3"

extern "C" [[noreturn]] void app_main(void) {
    const LED::Led led;

    for (uint8_t i = 0; i < 3; i++) {
        led.rainbow(50, 2);
    }
    led.set_color(LED::COLOR::BLACK);
}
