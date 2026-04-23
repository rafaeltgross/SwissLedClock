# LedBlink

Firmware for the ESP32-H2-DevKitC-1 that blinks the onboard WS2812 RGB LED 3 times on startup, then idles with the LED off.

## Hardware

| Item | Detail |
|------|--------|
| Board | ESP32-H2-DevKitC-1 |
| LED | Onboard WS2812 RGB (GPIO 8) |
| Framework | ESP-IDF 5.4 |

## Behavior

1. On power-on or reset, the LED starts off.
2. The LED blinks 3 times — 500 ms on, 500 ms off.
3. After the third blink the LED stays off indefinitely.

Total active duration: **3 seconds**.

## Configuration

All timing and pin constants are at the top of `main/led_blink_main.c`:

| Constant | Default | Description |
|----------|---------|-------------|
| `LED_PIN` | `8` | GPIO pin for the WS2812 LED |
| `LED_COLOR_R/G/B` | `16, 16, 16` | RGB color while LED is on |
| `LED_ON_MS` | `500` | ON duration per blink (ms) |
| `LED_OFF_MS` | `500` | OFF duration between blinks (ms) |
| `BLINK_COUNT` | `3` | Number of blinks |

## Build and Flash

```bash
. ~/esp/esp-idf/export.sh
idf.py -p /dev/ttyACM1 build flash
```

To monitor serial output after flashing:

```bash
idf.py -p /dev/ttyACM1 monitor
```

## Project Structure

```
LedBlink/
├── main/
│   ├── led_blink_main.c   # Application entry point
│   ├── CMakeLists.txt
│   └── idf_component.yml  # Declares espressif/led_strip dependency
├── documents/
│   ├── fsd.md             # Functional specification
│   └── blink-idea.md
├── CMakeLists.txt
└── sdkconfig.defaults
```

## Dependencies

- [`espressif/led_strip`](https://components.espressif.com/components/espressif/led_strip) — RMT-based WS2812 driver
