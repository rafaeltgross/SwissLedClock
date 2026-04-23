# Functional Specification Document: ESP32-H2 Swiss Clock LED

**Version:** 2.0
**Date:** 2026-04-24
**Project:** LedBlink — Swiss Clock
**Platform:** ESP32-H2-DevKitC-1

---

## 1. Purpose

This firmware turns the onboard WS2812 RGB LED of the ESP32-H2-DevKitC-1 into a visual Swiss clock chime. It uses LED blinks to signal the passage of time in quarter-hour increments, exactly replicating the strike pattern of a traditional Swiss clock.

---

## 2. Scope

- Quarter-hour LED chime sequence (1–3 blinks)
- Top-of-hour LED chime sequence (4 blinks + N hour strikes)
- 12-hour repeating cycle
- No RTC required — timing is software-based using FreeRTOS delays

Out of scope: UART logging, Wi-Fi/BLE, RTC sync, user input, multiple LEDs.

---

## 3. Hardware Requirements

| Item | Requirement |
|------|-------------|
| MCU | ESP32-H2-DevKitC-1 |
| LED | Onboard WS2812 RGB (GPIO 8) |
| Power | USB or 3.3 V regulated supply |
| Framework | ESP-IDF 5.4 |

---

## 4. Chime Pattern

The LED replicates the Westminster/Swiss quarter-hour chime logic:

| Time | Event | LED Blinks |
|------|-------|------------|
| HH:15 | Quarter 1 | 1 blink |
| HH:30 | Quarter 2 | 2 blinks |
| HH:45 | Quarter 3 | 3 blinks |
| HH:00 | Top of hour | 4 blinks, pause, then N blinks (where N = current hour 1–12) |

After the 12:00 chime (4 + 12 blinks) the cycle resets to 1:00.

---

## 5. Functional Requirements

### 5.1 Timing

| ID | Requirement |
|----|-------------|
| FR-01 | The firmware shall wait exactly `QUARTER_MS` (15 minutes) between chime events. |
| FR-02 | Each blink shall consist of LED ON for `BLINK_ON_MS` followed by LED OFF for `BLINK_OFF_MS`. |
| FR-03 | At the top of the hour, a pause of `CHIME_PAUSE_MS` shall separate the 4 quarter blinks from the hour strikes. |

### 5.2 Chime Sequence

| ID | Requirement |
|----|-------------|
| FR-04 | At quarter 1 (:15) the firmware shall blink 1 time. |
| FR-05 | At quarter 2 (:30) the firmware shall blink 2 times. |
| FR-06 | At quarter 3 (:45) the firmware shall blink 3 times. |
| FR-07 | At quarter 4 (:00) the firmware shall blink 4 times, pause, then blink N times where N is the current hour (1–12). |
| FR-08 | After the top-of-hour chime the hour counter shall advance by 1, wrapping from 12 back to 1. |

### 5.3 LED State

| ID | Requirement |
|----|-------------|
| FR-09 | The LED shall be OFF at startup and remain OFF between chime events. |
| FR-10 | The LED shall return to OFF after every blink. |

---

## 6. Configuration Constants

All user-tunable values are defined at the top of `main/led_blink_main.c`:

| Constant | Default | Description |
|----------|---------|-------------|
| `LED_PIN` | `8` | GPIO pin for the WS2812 LED |
| `LED_COLOR_R/G/B` | `20, 20, 20` | RGB color while LED is on (0–255) |
| `BLINK_ON_MS` | `200` | LED on duration per blink (ms) |
| `BLINK_OFF_MS` | `200` | LED off duration between blinks (ms) |
| `CHIME_PAUSE_MS` | `800` | Pause between quarter and hour chimes at top of hour (ms) |
| `QUARTER_MS` | `900000` | Interval between chime events = 15 min (ms) |

---

## 7. Behavioral Specification

### 7.1 State Diagram

```
[INIT] → [IDLE 15 min] → [CHIME] → [IDLE 15 min] → [CHIME] → ...
```

### 7.2 Chime Sequence at Top of Hour (e.g. 3:00)

```
LED: _|‾|_|‾|_|‾|_|‾|_  (pause)  _|‾|_|‾|_|‾|_
      ← 4 quarter blinks →          ← 3 hour strikes →
```

### 7.3 Full 12-Hour Cycle Summary

```
+0:15  → blink ×1
+0:30  → blink ×2
+0:45  → blink ×3
+1:00  → blink ×4, pause, blink ×1   (1 o'clock)
+1:15  → blink ×1
...
+12:00 → blink ×4, pause, blink ×12  (12 o'clock)
         → cycle resets to 1 o'clock
```

---

## 8. Non-Functional Requirements

| ID | Requirement |
|----|-------------|
| NFR-01 | All configurable values shall be defined as named constants at the top of the source file. |
| NFR-02 | Timing is software-based (FreeRTOS `vTaskDelay`). Accumulated drift over 12 hours is acceptable. |
| NFR-03 | The firmware shall compile and run on ESP-IDF 5.4 targeting ESP32-H2. |
| NFR-04 | The firmware shall use the `espressif/led_strip` component for WS2812 control. |

---

## 9. Acceptance Criteria

1. At :15 past any hour, exactly 1 blink is observed.
2. At :30, exactly 2 blinks; at :45, exactly 3 blinks.
3. At the top of the hour, exactly 4 blinks followed by a pause and N blinks (N = current hour).
4. After 12:00 (4+12 blinks), the next top-of-hour chime is 4+1 blinks.
5. The LED is off at all other times.
6. Changing `QUARTER_MS` to a shorter value (e.g. 5000 ms) allows rapid functional testing.
