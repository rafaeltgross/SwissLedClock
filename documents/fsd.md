# Functional Specification Document: ESP32-S3 LED Blink

**Version:** 1.0  
**Date:** 2026-04-23  
**Project:** LedBlink  
**Platform:** ESP32-S3  

---

## 1. Purpose

This document defines the functional requirements and expected behavior for the LedBlink firmware. The program serves as a hardware validation tool to confirm that an ESP32-S3 board is correctly programmed, that the development environment is functional, and that LED GPIO control is working.

---

## 2. Scope

This specification covers:
- LED blink sequence logic
- GPIO pin configuration
- Timing parameters
- Program termination behavior

It does not cover wireless, networking, UART logging, or any peripheral other than a single GPIO-controlled LED.

---

## 3. Hardware Requirements

| Item | Requirement |
|------|-------------|
| MCU | ESP32-S3 |
| LED | Onboard LED or external LED on a GPIO pin |
| GPIO | One configurable output pin |
| Power | USB or regulated 3.3V supply |

If using an external LED, a current-limiting resistor (typically 220–470 Ω) must be placed in series between the GPIO pin and the LED.

---

## 4. Functional Requirements

### 4.1 Blink Sequence

| ID | Requirement |
|----|-------------|
| FR-01 | The program shall blink the LED exactly **3 times** after startup. |
| FR-02 | Each blink shall consist of: LED ON → short delay → LED OFF → short delay. |
| FR-03 | The ON and OFF delays shall be equal and configurable via a single constant. |
| FR-04 | After the third blink completes, the program shall enter an idle state and perform no further GPIO activity. |

### 4.2 GPIO Configuration

| ID | Requirement |
|----|-------------|
| FR-05 | The LED GPIO pin number shall be defined as a named constant (e.g., `LED_PIN`). |
| FR-06 | The GPIO pin shall be initialized as a digital output before the blink sequence begins. |
| FR-07 | The LED shall be in the OFF state at program start and at program end. |

### 4.3 Timing

| ID | Requirement |
|----|-------------|
| FR-08 | The default ON delay shall be **500 ms**. |
| FR-09 | The default OFF delay shall be **500 ms**. |
| FR-10 | Delay values shall be defined as named constants to allow easy modification. |

---

## 5. Behavioral Specification

### 5.1 State Diagram

```
[INIT] → [BLINK_ON] → [BLINK_OFF] → (repeat 3×) → [IDLE]
```

### 5.2 Sequence Description

```
power-on / reset
│
├─ configure LED_PIN as output
├─ set LED_PIN LOW (LED OFF)
│
└─ loop 3 times:
    ├─ set LED_PIN HIGH  (LED ON)
    ├─ delay LED_ON_MS
    ├─ set LED_PIN LOW   (LED OFF)
    └─ delay LED_OFF_MS
│
└─ enter idle loop (no GPIO activity)
```

### 5.3 Timing Diagram

```
LED state:  _____|‾‾‾‾‾|_____|‾‾‾‾‾|_____|‾‾‾‾‾|_____  (idle)
             t0  +500ms +1s  +1.5s +2s  +2.5s +3s
```

Total active duration: 3 × (500 ms + 500 ms) = **3 000 ms** (3 seconds).

---

## 6. Configuration Constants

| Constant | Default Value | Description |
|----------|---------------|-------------|
| `LED_PIN` | Board-dependent | GPIO pin number for the LED |
| `LED_ON_MS` | 500 | Duration the LED stays ON (milliseconds) |
| `LED_OFF_MS` | 500 | Duration the LED stays OFF between blinks (milliseconds) |
| `BLINK_COUNT` | 3 | Number of times the LED blinks |

---

## 7. Non-Functional Requirements

| ID | Requirement |
|----|-------------|
| NFR-01 | The firmware shall compile and run under the ESP-IDF or Arduino framework for ESP32-S3. |
| NFR-02 | The program shall complete the blink sequence within 5 seconds of startup. |
| NFR-03 | The code shall be structured so that `LED_PIN`, `LED_ON_MS`, `LED_OFF_MS`, and `BLINK_COUNT` can be changed without modifying logic code. |
| NFR-04 | The program shall not rely on any external library beyond the ESP-IDF HAL or Arduino core. |

---

## 8. Acceptance Criteria

The implementation is considered correct when:

1. After flashing, the LED blinks exactly 3 times with visible ON/OFF transitions.
2. Each ON period is approximately 500 ms; each OFF period is approximately 500 ms.
3. After the third blink the LED remains OFF indefinitely.
4. Changing `BLINK_COUNT` to any positive integer causes exactly that many blinks.
5. Changing `LED_ON_MS` or `LED_OFF_MS` alters the blink timing accordingly.

---

## 9. Out of Scope

- Serial/UART debug output
- Wi-Fi or Bluetooth functionality
- Interrupts or RTOS tasks
- Multiple LEDs
- User input (buttons, touch)
