# PressGuard – Pressure Monitoring System for Industrial Press Machines

> Robust real-time monitoring and alert system for hydraulic press machines in the footwear industry.

---

## 📚 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [System Architecture](#system-architecture)
- [Hardware List](#hardware-list)
- [Wiring Diagram](#wiring-diagram)
- [How It Works](#how-it-works)
- [Software Structure](#software-structure)
- [Installation & Compilation](#installation--compilation)
- [Usage](#usage)
- [Maintenance & Safety](#maintenance--safety)
- [Troubleshooting](#troubleshooting)
- [License](#license)
- [Calibration](#calibration)
- [Author](#author)
---

## Overview

**PressGuard** is a real-time embedded system that monitors hydraulic pressure in industrial press machines, specifically designed for footwear manufacturing. It categorizes pressure conditions into three states (green, yellow, red) and uses lamps and a buzzer to alert operators of unsafe or suboptimal pressing conditions.

---

## Features

- **Real-Time Pressure Monitoring** using the MPX10DP sensor
- **Safety Integration** with the physical protective cover of the machine
- **Smart Alert Logic**:
  - Immediate alarm if pressure < 2 bar
  - Delayed alert if pressure < 5 bar after 3 seconds
- **Operator Feedback** through multi-color lamps and buzzer
- **Modular and Scalable Codebase** written in C++ (Arduino framework)
- **Ready for Expansion** to other industrial press systems or IoT networks

---

## System Architecture

- **Microcontroller**: ESP32 Dev Board
- **Pressure Sensor**: MPX10DP
- **Signal Amplifier**: LM358 Module (e.g., HW-685)
- **Power Converter**: Step-down (24V to 5V/3.3V)
- **Interfaces**:
  - 3 Indicator Lamps (Green, Yellow, Red)
  - 1 Buzzer (Alarm)
  - 1 Safety Sensor (detects lid position)

### Logical Flow

1. **Press Start Triggered** when the safety guard is lowered
2. **Pressure Sampling** begins
3. **Condition Evaluation**:
   - 5 bar: Green Lamp (Optimal)
   - 2–5 bar: Yellow Lamp (Suboptimal)
   - < 2 bar: Red Lamp + Buzzer (Critical)
   - After 3 seconds with < 5 bar: Warning Alert
4. **Alert Reset** on new pressing cycle

---

## Hardware List

| Component           | Description                       |
| ------------------- | --------------------------------- |
| ESP32 Dev Board     | Core microcontroller              |
| MPX10DP             | Pressure sensor                   |
| LM358 Amplifier     | Signal amplification              |
| Step-down Converter | 24V → 5V DC regulation            |
| LED Lamps           | Green, Yellow, Red indicators     |
| Buzzer              | Audio alert                       |
| Safety Sensor       | Detects guard position            |
| Misc. Electronics   | Wires, resistors, connectors, PCB |

---

## Wiring Diagram

```
24V Power Supply
    └──└── [Step-Down] ─────────────────────└──────└──────└── ESP32 (Vcc, GND)

MPX10DP → Amplifier (LM358) → ESP32 (ADC GPIO34)

Safety Sensor → ESP32 (Digital GPIO12)

Lamps & Buzzer → Relays / Drivers → ESP32 GPIOs

```

> Note: Ensure signal GND is common to ESP32 GND. Use opto-isolated drivers for 24V peripherals.

---

## How It Works

1. **Startup**: Self-test activates all lamps and buzzer briefly.
2. **Monitoring**:
   - The safety sensor detects the lid.
   - If pressed, the system samples analog pressure.
   - Logic processes thresholds:
     - 5 bar: Green
     - 2–5 bar: Yellow
     - < 2 bar: Red + Buzzer
     - < 5 bar for 3+ seconds: Warning state
3. **Alerts**: Managed through lamp states and buzzer pattern.
4. **Reset**: On lid reactivation, system resets alert states.

---

## Software Structure

Project uses modular C++ structure (Arduino-style):

- `main.cpp`: Initialization and loop logic
- `Lamp/`: Controls lamps and blink logic
- `Buzzer/`: Manages alert tones and patterns
- `PressureSensor/`: Converts analog readings to bar pressure
- `SecuritySensor/`: Tracks safety guard state and timers

> Logic separation allows scalability and easy maintenance.

---

## Installation & Compilation

### Requirements

- [PlatformIO](https://platformio.org/) or Arduino IDE (with ESP32 support)

### Steps

1. Clone this repository:

   ```bash
   git clone https://github.com/youruser/pressguard.git
   cd pressguard

   ```

2. Open with PlatformIO or Arduino IDE
3. Adjust pin mappings and pressure thresholds in `main.cpp`
4. Upload firmware to ESP32 via USB

---

## Usage

- Power the system (via 24V to 5V step-down)
- Wait for self-test to complete
- Start press cycle by lowering guard
- Observe indicators:
  - **Green**: Normal
  - **Yellow**: Monitor closely
  - **Red + Buzzer**: Stop and investigate

---

## Maintenance & Safety

- Periodically calibrate pressure sensor
- Inspect cables and connectors
- Verify step-down output stability
- Use proper enclosure and shielding in industrial environments
- Never bypass safety sensor

---

## Troubleshooting

| Problem                   | Possible Cause                       |
| ------------------------- | ------------------------------------ |
| No lamp/buzzer activity   | Wiring, power supply, or code issue  |
| Incorrect pressure values | Amplifier miscalibration or noise    |
| Constant alarm            | Faulty safety sensor or low pressure |

---

## Calibration

### Sensor and Amplifier Setup

- **Sensor**: MPX10DP (differential)
- **Amplifier**: LM358 module (e.g., HW-685)
- **ESP32 Input**: GPIO34 (ADC1)

### Wiring Overview

| MPX10DP Pin | LM358 Module | ESP32 Pin    |
| ----------- | ------------ | ------------ |
| 3 (+Vs)     | VCC (5V)     | —            |
| 1 (GND)     | GND          | GND          |
| 2 (+Vo)     | IN+          | —            |
| 4 (−Vo)     | IN−          | —            |
| —           | OUT          | GPIO34 (ADC) |

### Amplifier Adjustment

- **Offset (Zero)** Pot: Adjust until ~0.01866V at 0 bar
- **Gain (Span)** Pot: Adjust until ~0.26676V at 8 bar

### Pressure-to-Voltage Reference Table

| Pressure (BAR) | Output Voltage (V) |
| -------------- | ------------------ |
| 0              | 0.01866            |
| 1              | 0.09296            |
| 2              | 0.13836            |
| 3              | 0.16890            |
| 4              | 0.19740            |
| 5              | 0.21561            |
| 6              | 0.23354            |
| 7              | 0.24952            |
| 7.5            | 0.25500            |
| 8              | 0.26676            |

---

## 📄 License

MIT License — Free for personal and commercial use with attribution.

---

## Author

**Hendrius Félix Cerqueira Gomes de Santana**

Mechanical Engineer, Full Stack Developer, IoT & Industrial Automation Specialist

MBA in Software Engineering (PUC-Rio)

---
