# Real-Time LED Display Board

A Bluetooth-controlled P10 LED Display Board built using:

- Arduino Uno R3
- HC-05 Bluetooth Module
- DS3231 RTC Module
- 2 × P10 LED Matrix Displays (64×16)
- MIT App Inventor Android App

## Features

- Display scrolling custom messages
- Real-time clock display
- Real-time date display
- Update messages wirelessly via Bluetooth
- Update date and time from Android app
- Sync RTC with phone time
- Supports independent message and date/time updates

---

## Hardware Connections

### HC-05 Bluetooth Module

| HC-05 Pin | Arduino Uno |
|-----------|-------------|
| VCC | 5V |
| GND | GND |
| TXD | D2 |
| RXD | D3 |

---

### DS3231 RTC Module

| DS3231 Pin | Arduino Uno |
|------------|-------------|
| VCC | 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

---

### P10 LED Display (HUB12)

| P10 Pin | Arduino Uno |
|----------|-------------|
| A | D6 |
| B | D7 |
| CLK | D13 |
| LAT (L/SCLK) | D8 |
| OE (nOE) | D9 |
| DATA (R) | D11 |
| GND | GND |

---

## Power Connections

### P10 Display Power

> ⚠️ Do **NOT** power the P10 display directly from the Arduino 5V pin.

Use an external **5V power supply (minimum 3A recommended)**.

| Connection | Power Supply |
|------------|-------------|
| P10 VCC | +5V |
| P10 GND | GND |

Also connect:

| Connection |
|------------|
| External Power Supply GND → Arduino GND |

All grounds must be connected together.

---

## Complete Pin Usage Summary

| Arduino Pin | Connected To |
|-------------|-------------|
| D13 | P10 CLK |
| D11 | P10 DATA (R) |
| D9 | P10 OE |
| D8 | P10 LAT |
| D7 | P10 B |
| D6 | P10 A |
| D3 | HC-05 RX |
| D2 | HC-05 TX |
| A5 | DS3231 SCL |
| A4 | DS3231 SDA |

---

## Bluetooth Command Format

### Update Message

```text
*Swaraj Keshari#
```

Updates only the scrolling message.

---

### Update Time Only

```text
#@01/02/33,DD/MM/YY
```

Updates only the RTC time.

---

### Update Date Only

```text
#@HH/MM/SS,20/06/26
```

Updates only the RTC date.

---

### Update Date and Time

```text
#@01/02/33,20/06/26
```

Updates both date and time.

---

## Android Application

The Android controller app is built using MIT App Inventor and provides:

- Bluetooth device connection
- Custom message transmission
- Manual date and time setting
- Automatic phone time synchronization

---

## Project Name

**Real-Time LED Display Board**
