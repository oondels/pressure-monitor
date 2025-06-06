# Pressure Monitoring System for Industrial Press Machines

> **Robust real-time monitoring and alert system for hydraulic press machines in the footwear industry.**

---

## Table of Contents

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
- [Author](#author)

---

## Overview

**PressGuard** is an embedded solution designed to monitor the hydraulic pressure of industrial press machines, specifically targeting the footwear sector. The system provides real-time pressure status indication via colored lamps (Green, Yellow, Red) and activates audio and visual alarms to prevent defective production due to insufficient pressing force.

---

## Features

- **Real-Time Pressure Monitoring**: Continuously samples pressure with the MPX10DP sensor.
- **Safety Integration**: Works with the machine’s physical safety guard (safety sensor).
- **Intelligent Alert Logic**:
  - **Instant alarm** if pressure < 2 bar.
  - **Preemptive warning** if pressure < 5 bar after 3 seconds from pressing cycle start.
- **Operator Feedback**: Visual (multi-color lamps) and audio (buzzer) alerts.
- **Robust Modular Codebase**: Fully object-oriented C++ for maintainability and scalability.
- **Scalable**: Easily adapted for other press types or process automation.

---

## System Architecture

- **Microcontroller**: ESP32 Dev Board
- **Pressure Sensor**: MPX10DP
- **Signal Amplifier**: For analog sensor reading
- **Step-down Converter**: Reduces 24V machine supply to 5V/3.3V for ESP32 and peripherals
- **Interface**:
  - 3 lamps (Red/Yellow/Green)
  - 1 Buzzer (alert)
  - 1 Safety Sensor input (detects guard position)

**Logical Flow:**

1. **Safety Guard Down:** Press cycle starts when safety guard sensor is triggered.
2. **Pressure Monitoring:** System samples and evaluates pressure via MPX10DP.
3. **Status Indication:**
   - Green lamp if pressure > 5 bar.
   - Yellow lamp if 2 < pressure < 5 bar.
   - Red lamp + buzzer if pressure < 2 bar (immediate) or < 5 bar after 3 seconds.
4. **Alerts:** Operator is immediately notified if pressure is inadequate for safe pressing.

---

## Hardware List

| Component             | Description                                   |
| --------------------- | --------------------------------------------- |
| ESP32 Dev Board       | Main controller                               |
| MPX10DP               | Pressure sensor                               |
| Signal Amplifier      | For MPX10DP (e.g., LM358 module)              |
| Step-down Converter   | 24V → 5V (e.g., LM2596)                       |
| Lamps (LED + driver)  | Red, Yellow, Green (e.g., 24V relays or opto) |
| Buzzer                | 5V/12V buzzer                                 |
| Safety Sensor         | ----------------------------                  |
| Wires, resistors, PCB | As needed                                     |

---

## Wiring Diagram

---

## How It Works

1. **Initialization:**
   - On power-up, all components are tested (lamps and buzzer).
2. **Operation:**
   - The system enters monitoring mode.
   - When the safety guard is detected as "down", the ESP32 starts sampling pressure.
   - Pressure is evaluated and categorized:
     - **Green:** > 5 bar
     - **Yellow:** 2 – 5 bar
     - **Red/Buzzer:** < 2 bar (immediate alarm)
     - If < 5 bar after 3 seconds, warning alert is triggered.
   - Lamps are mutually exclusive; only one is on at a time, except during alerts (red lamp blinks).
3. **Alert Reset:**
   - Alerts reset automatically if pressing cycle is restarted.

---

## Software Structure

**Modular, object-oriented C++ (Arduino framework):**

- `main.cpp`: Initialization and main loop logic.
- `/Lamp`: Manages lamp states, exclusive operation, and alert blinking.
- `/Buzzer`: Controls buzzer alerts, with support for timed beeps.
- `/PressureSensor`: Reads and maps analog input to physical pressure values (bar).
- `/SecuritySensor`: Detects safety guard state and manages timing for alerts.

**Key Logic:**

- All business rules for alerting and lamp logic are encapsulated in their respective classes.
- Thresholds and timings are clearly defined and easy to adjust.

---

## Installation & Compilation

### Prerequisites

- [PlatformIO](https://platformio.org/) or Arduino IDE configured for ESP32.
- Proper wiring according to diagram.

### Steps

1. **Clone this repository**
   ```sh
   git clone https://github.com/youruser/pressguard.git
   cd pressguard
   ```
2. **Open in PlatformIO or Arduino IDE**
3. **Connect your ESP32 via USB**
4. **Adjust `main.cpp` pin definitions and pressure thresholds as needed**
5. **Build and upload to ESP32**

---

## Usage

- **Power on** the system; lamps and buzzer will self-test.
- **Start machine cycle** by lowering safety guard.
- **Observe lamps:**
  - **Green:** Optimal pressure, proceed with pressing.
  - **Yellow:** Sub-optimal pressure, check hydraulic system.
  - **Red/Buzzer:** Dangerous low pressure — stop and investigate immediately.
- **Respond to alerts** as instructed to avoid faulty production or equipment damage.

---

## Maintenance & Safety

- **Regularly calibrate the pressure sensor** for accurate readings.
- **Visually inspect wiring and connectors** for wear or damage.
- **Ensure step-down and relays are rated for industrial environment.**
- **Never bypass safety mechanisms.**
- **Test system at least once per production shift.**

---

## Troubleshooting

- **No lamp or buzzer activity:**
  - Check 24V supply and step-down output.
  - Inspect all wiring and connectors.
  - Confirm ESP32 firmware upload.
- **Erratic pressure readings:**
  - Check amplifier circuit and sensor calibration.
  - Ensure signal integrity and proper analog reference.
- **Continuous alert:**

  - Verify hydraulic supply and safety sensor status.

  # Calibração – Sensor de Pressão MPX10GP com Amplificador HW-685

## 🔧 Configuração Física

- **Sensor:** MPX10GP (diferencial, não compensado)
- **Amplificador:** HW-685 (baseado em LM358)
- **Microcontrolador:** ESP32
- **Leitura de sinal:** ADC de 12 bits (0–4095), 3.3V de referência

## 🔌 Esquema de Conexão

| MPX10GP     | HW-685 (LM358) | ESP32         |
| ----------- | -------------- | ------------- |
| Pin 3 (+Vs) | VCC (5V)       | —             |
| Pin 1 (GND) | GND            | GND           |
| Pin 2 (+Vo) | IN+            | —             |
| Pin 4 (−Vo) | IN−            | —             |
| —           | OUT            | GPIO34 (ADC1) |

> Obs.: OUT do HW-685 fornece tensão analógica amplificada proporcional à pressão diferencial.

---

## ⚙️ Ajustes do Amplificador (HW-685)

O HW-685 possui dois trimpots ajustáveis:

- **Zero (Offset):** define o nível de saída quando a pressão diferencial é 0.
  - **Valor calibrado:** `3.6 kΩ` (ajustado até a leitura estabilizar em ~0.01866V na pressão atmosférica)
- **Span (Ganho):** define quanto a tensão de saída varia conforme a pressão.
  - **Valor calibrado:** `9.62 kΩ` (ajustado para que a saída chegue até ~0.26676V em 8 BAR)

### Resultado da Calibração

| Pressão (BAR) | Tensão Média (V) |
| ------------- | ---------------- |
| 0             | 0.01866          |
| 1             | 0.09296          |
| 2             | 0.13836          |
| 3             | 0.16890          |
| 4             | 0.19740          |
| 5             | 0.21561          |
| 6             | 0.23354          |
| 6.5           | 0.24             |
| 7             | 0.24952          |
| 7.5           | 0.2550           |
| 8             | 0.26676          |

---

## Author

**Hendrius Félix Cerqueira Gomes de Santana**  
Mechanical Engineer, Software Developer (PUC-Rio), IoT & Industrial Automation Specialist.

---
