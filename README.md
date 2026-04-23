# LedBlink — Swiss Clock

The onboard RGB LED on your ESP32-H2 blinks like a Swiss clock — once every 15 minutes to mark the quarter hours, and at the top of every hour it strikes four times plus once for each hour (just like the bells on a real clock).

## How it works

| Time | What you see |
|------|--------------|
| :15 past | 1 blink |
| :30 past | 2 blinks |
| :45 past | 3 blinks |
| Top of hour | 4 blinks, short pause, then N blinks for the hour (1–12) |

After 12 o'clock the cycle starts over at 1.

## Hardware

- **Board:** ESP32-H2-DevKitC-1
- **LED:** Onboard WS2812 RGB on GPIO 8
- **Framework:** ESP-IDF 5.4

## Build and flash

```bash
. ~/esp/esp-idf/export.sh
idf.py -p /dev/ttyACM1 build flash
```

## Customise

Open `main/led_blink_main.c` and change the constants at the top:

| Constant | Default | What it does |
|----------|---------|--------------|
| `BLINK_ON_MS` | 200 ms | How long the LED stays on per blink |
| `BLINK_OFF_MS` | 200 ms | Gap between blinks |
| `CHIME_PAUSE_MS` | 800 ms | Pause between quarter and hour chimes |
| `QUARTER_MS` | 15 min | How often the clock chimes |
| `LED_COLOR_R/G/B` | 20, 20, 20 | LED colour (0–255 each) |

> **Tip:** Set `QUARTER_MS` to `5000` (5 seconds) for quick testing without waiting 15 minutes.

## Project files

```
main/led_blink_main.c   — firmware source
documents/fsd.md        — full functional specification
documents/blink-idea.md — original project notes
```
