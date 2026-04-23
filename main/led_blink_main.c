#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"

/* Configuration — change only here */
#define LED_PIN           8       /* GPIO8 = onboard WS2812 on ESP32-H2-DevKitC-1 */
#define LED_COLOR_R       20
#define LED_COLOR_G       20
#define LED_COLOR_B       20
#define BLINK_ON_MS       200
#define BLINK_OFF_MS      200
#define CHIME_PAUSE_MS    800
#define QUARTER_CHIMES    4
#define QUARTER_MS        (15UL * 60UL * 1000UL)

static void blink(led_strip_handle_t strip, int count)
{
    for (int i = 0; i < count; i++) {
        ESP_ERROR_CHECK(led_strip_set_pixel(strip, 0, LED_COLOR_R, LED_COLOR_G, LED_COLOR_B));
        ESP_ERROR_CHECK(led_strip_refresh(strip));
        vTaskDelay(pdMS_TO_TICKS(BLINK_ON_MS));
        ESP_ERROR_CHECK(led_strip_clear(strip));
        if (i < count - 1)
            vTaskDelay(pdMS_TO_TICKS(BLINK_OFF_MS));
    }
}

void app_main(void)
{
    led_strip_config_t strip_cfg = {
        .strip_gpio_num = LED_PIN,
        .max_leds = 1,
    };
    led_strip_rmt_config_t rmt_cfg = {
        .resolution_hz = 10 * 1000 * 1000,
        .flags.with_dma = false,
    };
    led_strip_handle_t strip;
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_cfg, &rmt_cfg, &strip));
    ESP_ERROR_CHECK(led_strip_clear(strip));

    int hour = 12;
    int quarter = 0;
    TickType_t wake = xTaskGetTickCount();

    while (1) {
        vTaskDelayUntil(&wake, pdMS_TO_TICKS(QUARTER_MS));

        quarter++;
        if (quarter == QUARTER_CHIMES) {
            hour = (hour % 12) + 1;
            blink(strip, QUARTER_CHIMES);
            vTaskDelay(pdMS_TO_TICKS(CHIME_PAUSE_MS));
            blink(strip, hour);
            quarter = 0;
        } else {
            blink(strip, quarter);
        }
    }
}
