# Pressure Monitor – Pressure Monitoring System for Industrial Press Machines

> Robust real-time monitoring and alert system for hydraulic press machines in the footwear industry.

[![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP32-orange)](https://platformio.org/)
[![Framework](https://img.shields.io/badge/Framework-Arduino-blue)](https://www.arduino.cc/)
[![Language](https://img.shields.io/badge/Language-C%2B%2B-red)](https://isocpp.org/)

---

## 📚 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [System Architecture](#system-architecture)
- [Hardware Specifications](#hardware-specifications)
- [Pin Configuration](#pin-configuration)
- [Software Architecture](#software-architecture)
- [State Machine Logic](#state-machine-logic)
- [Installation & Setup](#installation--setup)
- [Configuration](#configuration)
- [Usage & Operation](#usage--operation)
- [Calibration Guide](#calibration-guide)
- [API Reference](#api-reference)
- [Troubleshooting](#troubleshooting)
- [Performance & Optimization](#performance--optimization)
- [Contributing](#contributing)
- [Author](#author)
---

## Overview

**Pressure Monitor** is a real-time embedded system that monitors pressure in industrial press machines, specifically engineered for footwear manufacturing environments. The system implements intelligent state machine logic to categorize pressure conditions into three operational states and provides immediate feedback through visual and auditory alerts to ensure operator safety and production quality.

The system features advanced safety integration with the machine's protective cover, implementing time-based alert logic to prevent false alarms while ensuring rapid response to genuine safety concerns.

---

## Features

### Core Functionality
- **High-Precision Pressure Monitoring** using MPX10DP differential pressure sensor
- **Advanced Signal Processing** with LM358 amplifier and multi-point calibration
- **Intelligent Safety Integration** with protective cover position detection
- **State Machine Architecture** for robust operation state management

### Smart Alert System
- **Immediate Response**: Red lamp blink + buzzer for pressure < 2 bar
- **Progressive Alerts**: Yellow lamp for pressure between 2-5 bar
- **Time-Based Logic**: Delayed alert (3 seconds) for pressure < 5 bar to prevent false alarms
- **Auto-Reset Capability**: System resets on guard reactivation

### Technical Excellence
- **Object-Oriented Design** with modular C++ architecture
- **Polymorphic Alert System** using interface-based design patterns
- **Industrial-Grade Reliability** with robust error handling and fail-safe mechanisms

---

## System Architecture

**Pressure Monitor** implements a layered architecture with clear separation of concerns:

### Hardware Layer
- **Microcontroller**: ESP32 DevKit V1
- **Pressure Sensor**: MPX10DP Differential Pressure Sensor
- **Signal Amplifier**: LM358 Op-Amp Module (HW-685)
- **Power Management**: 24V to 5V/3.3V Step-down Converter
- **User Interface**: 3 LED Indicators + Buzzer
- **Safety Input**: Digital Sensor for Guard Position Detection

### Software Architecture
- **Main Controller**: Orchestrates all system components
- **Sensor Management**: Pressure reading and safety monitoring
- **Alert System**: Polymorphic interface for flexible alerting
- **State Machine**: Manages operational states and transitions
- **Message System**: Handles logging and future IoT integration

### Operational Flow

1. **System Initialization**: Self-test sequence validates all components
2. **Safety Monitoring**: Continuous guard position detection
3. **Pressure Sampling**: Multi-sample averaging for accuracy
4. **State Evaluation**: Intelligent thresholds with time-based logic
5. **Alert Management**: Coordinated visual and auditory feedback
6. **Auto-Reset**: Seamless cycle restart on guard reactivation

---

## Hardware Specifications

| Component | Model/Type | Function | Interface |
|-----------|------------|----------|-----------|
| **Microcontroller** | ESP32 DevKit V1 | Main processing unit | Arduino Framework |
| **Pressure Sensor** | MPX10DP | Differential pressure measurement | Analog (0-3.3V) |
| **Signal Amplifier** | LM358 (HW-685) | Signal conditioning | Op-Amp Module |
| **Power Supply** | Step-down Converter | 24V DC → 5V/3.3V | Buck Converter |
| **Green Indicator** | LED Lamp | Optimal pressure status | Digital Output |
| **Yellow Indicator** | LED Lamp | Warning pressure status | Digital Output |
| **Red Indicator** | LED Lamp | Critical pressure alert | Digital Output |
| **Audio Alert** | Buzzer | Auditory alarm | Digital Output |
| **Safety Sensor** | Digital Switch | Guard position detection | Digital Input |

---

## Pin Configuration

### ESP32 Pin Assignments

| GPIO Pin | Component | Type | Description |
|----------|-----------|------|-------------|
| **GPIO 34** | Pressure Sensor | Analog Input | ADC1_CH6 - Pressure signal |
| **GPIO 5** | Safety Sensor | Digital Input | Guard position detection |
| **GPIO 26** | Green Lamp | Digital Output | Optimal pressure indicator |
| **GPIO 25** | Yellow Lamp | Digital Output | Warning pressure indicator |
| **GPIO 27** | Red Lamp | Digital Output | Critical pressure indicator |
| **GPIO 14** | Buzzer | Digital Output | Audio alert system |

### Power Distribution

```
24V Industrial Supply
    ↓
Step-Down Converter
    ├── 5V → Sensors & Amplifier
    └── 3.3V → ESP32 Logic Level
```

---

## Software Architecture

The Pressure Monitor system follows object-oriented design principles with clear separation of concerns:

### Core Classes

#### `PressureSensor` Class
- **Purpose**: Handles analog pressure signal reading and conversion
- **Key Features**:
  - Multi-sample averaging (configurable sample count)
  - Piecewise linear calibration curve
  - Voltage-to-pressure conversion using lookup table
  - Static members for memory efficiency

#### `SecuritySensor` Class
- **Purpose**: Manages safety guard position detection and timing logic
- **State Machine**: Implements three states (INACTIVE, ACTIVE, ALERT)
- **Key Features**:
  - Time-based alert triggering (3-second delay)
  - Auto-reset on guard reactivation
  - Debounced digital input handling

#### `Lamp` Class
- **Purpose**: Controls LED indicators with smart switching logic
- **Key Features**:
  - Static lamp registry for centralized control
  - Automatic mutual exclusion (only one lamp active)
  - Blinking mode for critical alerts
  - Implements `IAlertDevice` interface

#### `Buzzer` Class
- **Purpose**: Manages audio alerts with configurable patterns
- **Key Features**:
  - Beeping pattern generation
  - Non-blocking operation
  - Implements `IAlertDevice` interface

#### `IAlertDevice` Interface
- **Purpose**: Polymorphic alert system for extensibility
- **Key Methods**: `triggerAlert()`, `reset()`
- **Compound Alert**: Allows multiple devices to be controlled as one unit

### System States and Logic

#### Pressure Thresholds
```cpp
#define LOW_PRESSURE_THRESHOLD 2.0   // Critical level
#define HIGH_PRESSURE_THRESHOLD 5.0  // Optimal level
```

#### State Transitions
1. **Guard Down** → System becomes ACTIVE
2. **Pressure < 2 bar** → Immediate red lamp blink + buzzer
3. **Pressure 2-5 bar** → Yellow lamp steady
4. **Pressure ≥ 5 bar** → Green lamp steady
5. **Guard Up** → System RESET

#### Time-Based Safety Logic
- **3-second delay**: Before triggering alert for pressure < 5 bar
- **4-second timeout**: Maximum alert duration before auto-reset
- **50ms main loop**: Ensures responsive system behavior

---

## State Machine Logic

The SecuritySensor implements a sophisticated state machine:

```
[INACTIVE] ──guard_down──> [ACTIVE] ──timeout_or_low_pressure──> [ALERT]
     ↑                         ↑                                    │
     └─────────────────────────┴────────────guard_up_or_timeout────┘
```

### State Descriptions

- **INACTIVE**: System idle, waiting for guard activation
- **ACTIVE**: Monitoring pressure, normal operation
- **ALERT**: Safety condition triggered, alerts active

### Alert Triggering Conditions

1. **Immediate Alert**: Pressure < 2 bar (red blink + buzzer)
2. **Delayed Alert**: Pressure < 5 bar for > 3 seconds
3. **Auto-Reset**: Guard reactivation or 4-second timeout

---

## Installation & Setup

### Prerequisites

- [PlatformIO](https://platformio.org/) or Arduino IDE with ESP32 support
- ESP32 DevKit V1 board
- PlatformIO CLI (optional, for command-line builds)

### Hardware Assembly

1. **Power Supply Setup**:
   - Connect 24V industrial supply to step-down converter
   - Verify 5V and 3.3V outputs are stable
   - Ensure proper grounding

2. **Sensor Connections**:
   - Wire MPX10DP to LM358 amplifier module
   - Connect amplifier output to ESP32 GPIO34
   - Verify signal levels are within 0-3.3V range

3. **I/O Connections**:
   - Connect lamps to GPIOs 26, 25, 27 (use appropriate drivers)
   - Connect buzzer to GPIO14
   - Connect safety sensor to GPIO5

### Software Installation

1. **Clone Repository**:
   ```bash
   git clone https://github.com/oondels/pressure-monitor
   cd pressure-monitor
   ```

2. **PlatformIO Build**:
   ```bash
   pio run                    # Build project
   pio run --target upload    # Upload to ESP32
   pio device monitor        # View serial output
   ```

3. **Arduino IDE Setup**:
   - Install ESP32 board support
   - Select "ESP32 Dev Module" board
   - Set upload speed to 115200
   - Compile and upload

### Configuration Parameters

Adjust these values in `main.cpp` as needed:

```cpp
float max_pressure = 8.0;        // Maximum expected pressure
float min_pressure = 2.0;        // Minimum safe pressure  
float operatingPressure = 5.0;   // Optimal operating pressure
```

---

## Configuration

### Pressure Calibration Settings

The system uses a piecewise linear calibration curve. Current settings:

```cpp
// Voltage-to-Pressure Mapping Points
{0.01866V, 0.0 bar}, {0.09296V, 1.0 bar}, {0.13836V, 2.0 bar},
{0.16890V, 3.0 bar}, {0.19740V, 4.0 bar}, {0.21561V, 5.0 bar},
{0.23354V, 6.0 bar}, {0.24952V, 7.0 bar}, {0.26676V, 8.0 bar}
```

### Timing Parameters

```cpp
limitActiveTime = 3000;    // 3 seconds before alert
limitAlertTime = 4000;     // 4 seconds maximum alert duration
blinkInterval = 500;       // 500ms blink rate for red lamp
beepInterval = 500;        // 500ms beep interval for buzzer
```

### System Monitoring

Enable serial output for debugging:
```cpp
// Uncomment in main.cpp loop()
messageManager->printMessage("Pressure: " + String(pressure));
```

---

## Usage & Operation

### Startup Sequence

1. **Power On**: System initializes all components
2. **Self-Test**: All lamps and buzzer activate briefly
3. **Ready State**: System waits for guard activation

### Normal Operation

1. **Lower Safety Guard**: Activates pressure monitoring
2. **Observe Indicators**:
   - **Green**: Pressure ≥ 5 bar (optimal)
   - **Yellow**: Pressure 2-5 bar (suboptimal)
   - **Red Blinking + Buzzer**: Pressure < 2 bar (critical)
3. **Raise Guard**: Resets system for next cycle

### Emergency Procedures

- **Critical Pressure Alert**: Stop operation immediately
- **System Reset**: Raise and lower guard to reset alerts
- **Power Cycle**: If system becomes unresponsive

### Maintenance Indicators

- **Erratic Readings**: Check sensor connections
- **No Response**: Verify power supply stability
- **False Alarms**: Recalibrate pressure sensor

---

## API Reference

### Core API Functions

#### PressureSensor API
```cpp
// Constructor
PressureSensor(int pin, float maxPressure, float minPressure, float operatingPressure);

// Methods
void readSensorSignal(int samples);    // Read and average sensor samples
float getPressure();                   // Get calibrated pressure value
float getMaxPressure() const;          // Get maximum pressure threshold
float getMinPressure() const;          // Get minimum pressure threshold
```

#### SecuritySensor API
```cpp
// Constructor
SecuritySensor(int pin);

// Methods  
bool isActive();                       // Check if guard is down
bool isAlert();                        // Check if in alert state
void reset();                          // Reset sensor state
unsigned long getActiveTime();         // Get time since activation
unsigned long getAlertTime();          // Get time since alert started

// Static method
static void watchSensor(IAlertDevice& alertDevice, 
                       PressureSensor* pressureSensor, 
                       SecuritySensor* securitySensor);
```

#### Lamp API
```cpp
// Constructor
Lamp(int pin, const char* name);

// Methods
void turnOn();                         // Turn on this lamp (turns off others)
void reset() override;                 // Turn off all lamps
void triggerAlert() override;          // Activate blinking mode

// Static methods
static void test();                    // Test all lamps sequentially
static Lamp* getLampByName(const char* lampName);
static void toggleLeds(float pressure, SecuritySensor* securitySensor);
```

#### Buzzer API
```cpp
// Constructor
Buzzer(int pin);

// Methods
void turnOn();                         // Turn on buzzer
void turnOff();                        // Turn off buzzer
void test();                           // Test buzzer functionality
void reset() override;                 // Reset buzzer state
void triggerAlert() override;          // Activate beeping pattern
```

#### IAlertDevice Interface
```cpp
// Pure virtual interface
virtual void reset() = 0;              // Reset alert state
virtual void triggerAlert() = 0;       // Trigger alert
```

#### CompoundAlert API
```cpp
// Constructor
CompoundAlert(IAlertDevice** devices, size_t count);

// Methods
void reset() override;                 // Reset all devices
void triggerAlert() override;          // Trigger all devices
```

---

## Troubleshooting

### Common Issues and Solutions

| Problem | Symptoms | Possible Causes | Solutions |
|---------|----------|----------------|-----------|
| **No Lamp Activity** | All lamps remain off | Power supply, wiring, or GPIO configuration | Check 24V→5V converter, verify pin connections, test GPIO outputs |
| **Incorrect Pressure Readings** | Erratic or constant values | Sensor miscalibration, amplifier drift, noise | Recalibrate using reference pressure, adjust amplifier trimpots, add filtering |
| **Constant Alarm** | Red lamp always blinking + buzzer | Faulty safety sensor or genuine low pressure | Check safety sensor wiring, verify actual pressure levels, test sensor logic |
| **No Response to Guard** | System doesn't activate when guard lowered | Safety sensor malfunction, pin configuration | Verify GPIO5 connection, test digital input, check sensor power |
| **False Alarms** | Random alerts without cause | Electrical noise, timing issues, sensor drift | Add debouncing, check grounding, verify timing parameters |
| **System Freezes** | No response to inputs | Memory issues, infinite loops, hardware fault | Monitor serial output, check for memory leaks, verify power stability |

### Diagnostic Commands

Enable debug output by uncommenting in `main.cpp`:
```cpp
messageManager->printMessage("Pressure: " + String(pressure));
```

### Hardware Testing

1. **Pressure Sensor Test**:
   - Apply known pressure values
   - Verify voltage output matches calibration table
   - Check for stable readings

2. **Safety Sensor Test**:
   - Manually activate/deactivate
   - Verify digital signal changes
   - Test timing logic

3. **Output Device Test**:
   - Use built-in test functions
   - Verify all lamps and buzzer operate
   - Check switching logic

---

## Performance & Optimization

### System Performance Metrics

- **Main Loop Rate**: 50ms (20 Hz)
- **Pressure Sampling**: 20 samples averaged over 200ms
- **Alert Response Time**: < 100ms for critical conditions
- **Memory Usage**: ~2KB static allocation
- **Power Consumption**: < 200mA @ 5V (excluding external devices)

### Optimization Guidelines

1. **Memory Efficiency**:
   - Static class members reduce RAM usage
   - Minimal dynamic allocation
   - Efficient data structures

2. **Processing Optimization**:
   - Non-blocking operations
   - Efficient calibration lookup
   - Minimal floating-point operations

3. **Real-time Performance**:
   - Predictable execution times
   - Priority-based task scheduling
   - Hardware interrupt handling

### Scalability Considerations

- **IoT Integration**: Message system ready for network expansion
- **Multi-sensor Support**: Architecture supports additional sensors
- **Remote Monitoring**: Extensible alert system for cloud connectivity
- **Data Logging**: Built-in message manager for data collection

---

## Contributing

We welcome contributions to improve the project! Please follow these guidelines:

### Development Setup

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/your-feature`
3. Install PlatformIO development environment
4. Make your changes with proper documentation
5. Test thoroughly on actual hardware
6. Submit a pull request

### Bug Reports

Please include:
- Hardware configuration
- Software version
- Detailed steps to reproduce
- Expected vs. actual behavior
- Serial output logs if available

---

## Calibration Guide

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

The amplifier configuration used is:
- **Zero (Offset)** Pot: 3.6kΩ → Output ~0.01866V at 0 bar
- **Span (Gain)** Pot: 9.62kΩ → Output ~0.26676V at 8 bar

### Pressure-to-Voltage Reference Table

| Pressure (BAR) | Output Voltage (V) | Used in Code |
| -------------- | ------------------ | ------------ |
| 0              | 0.01866            | ✓            |
| 1              | 0.09296            | ✓            |
| 2              | 0.13836            | ✓            |
| 3              | 0.16890            | ✓            |
| 4              | 0.19740            | ✓            |
| 5              | 0.21561            | ✓            |
| 6              | 0.23354            | ✓            |
| 7              | 0.24952            | ✓            |
| 7.5            | 0.25500            | ✓            |
| 8              | 0.26676            | Max Range    |

### Calibration Process

1. **Zero Calibration**:
   - Apply 0 bar pressure (atmospheric)
   - Adjust zero pot until voltage reads ~0.01866V
   - Verify stability over 30 seconds

2. **Span Calibration**:
   - Apply full-scale pressure (8 bar)
   - Adjust span pot until voltage reads ~0.26676V
   - Re-check zero point and repeat if necessary

3. **Linearity Verification**:
   - Test intermediate pressure points
   - Verify readings match the calibration table
   - Update code constants if needed

### Code Implementation

The pressure conversion uses piecewise linear interpolation:

```cpp
float PressureSensor::getPressure() {
  if (sensorSignal <= 0.0187) {
    pressure = 0.0;
  }
  else if (sensorSignal <= 0.09296) {
    pressure = mapFloat(sensorSignal, 0.01866, 0.09296, 0.0, 1.0);
  }
  // ... additional ranges
  else {
    pressure = 8.0; // Maximum pressure
  }
  return pressure;
}
```

---

## Author

**Hendrius Félix Cerqueira Gomes de Santana**

Mechanical Engineer, Back-End Developer, IoT & Industrial Automation Enthusiast

MBA in Software Engineering (PUC-Rio)

---
