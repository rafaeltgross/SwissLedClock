/* Configuration constants — change only here (NFR-03)
 * NOTE: GPIO48 on ESP32-S3-DevKitC-1 is an addressable RGB LED (WS2812).
 * For plain GPIO blink, connect an external LED + 220-470 Ohm resistor
 * to a free GPIO (e.g. 2, 4, 8) and update LED_PIN accordingly. */
#define LED_PIN      48
#define LED_ON_MS    500
#define LED_OFF_MS   500
#define BLINK_COUNT  3

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

void app_main(void)
{
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 0);

    for (int i = 0; i < BLINK_COUNT; i++) {
        gpio_set_level(LED_PIN, 1);
        vTaskDelay(LED_ON_MS / portTICK_PERIOD_MS);
        gpio_set_level(LED_PIN, 0);
        vTaskDelay(LED_OFF_MS / portTICK_PERIOD_MS);
    }

    while (1) {
        vTaskDelay(portMAX_DELAY);
    }
}
