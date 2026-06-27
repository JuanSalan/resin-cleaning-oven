# Resin-cleaning oven

ESP32-based oven control system, developed as part of a mechatronics engineering project. It integrates PID control, multi-channel temperature sensing, a local interface (LCD + keypad), and remote logging via Bluetooth.

## Overview

Horno_PAI is an embedded thermal control system that combines hardware design (PCB in KiCad) and firmware (ESP32) to control an oven through an SSR (Solid State Relay), with simultaneous monitoring of up to four temperature points. The project covers the full development cycle: from circuit design and component selection, to the control algorithm and experimental validation.

## Key Features

- **PID control** with time-proportional output for driving the SSR.
- **Multi-channel temperature measurement**: 4 type-K thermocouples via MAX6675 modules.
- **Local interface**: LCD display and matrix keypad via PCF8574 I/O expander.
- **Remote logging**: data sent over Bluetooth Classic (SPP) using `BluetoothSerial`.
- **Non-blocking loop**: `millis()`-based architecture instead of `delay()`.
- **Hardware protections**: AMS1117-3.3V regulator, reverse polarity protection, polyfuse.
- **Indicators and alarms**: status LEDs and an NPN transistor-driven buzzer.

## Hardware

### Main Components

| Component | Function |
|---|---|
| ESP32 | Main microcontroller |
| MAX6675 (x4) | Type-K thermocouple reading |
| SSR | Drives the heating element |
| PCF8574 | I/O expander for the matrix keypad |
| LCD | Local display interface |
| AMS1117-3.3V | Voltage regulation |
| Buzzer + NPN transistor | Audible alarms |

### PCB Design

The board was designed in **KiCad 9**, including:
- GPIO assignment for the 4 thermocouples, SSR, LCD, keypad, and peripherals.
- Footprint selection and trace widths sized for expected currents.
- Autorouting assisted by FreeRouting.

>  **Known design note:** the control stage and the power stage share power and ground, which introduces noise in the thermocouple readings. This is a hardware-rooted issue currently mitigated with software filtering (see Known Issues below).

## Firmware

The firmware is written in C++ (Arduino framework) for the ESP32, with the following modules:

- **PID control**: error calculation and control output, translated into a time-proportional signal for the SSR.
- **Temperature acquisition**: periodic reading of the 4 MAX6675 channels.
- **User interface**: keypad handling (PCF8574) and LCD updates.
- **Communication**: process variable logging over Bluetooth Classic SPP.
- **Non-blocking architecture**: all periodic tasks are managed with `millis()`-based timing.

## Repository Structure

```
Horno_PAI/
├── hardware/
│   ├── kicad/              # Schematics and PCB (KiCad 9)
│   └── datasheets/         # Datasheets for key components
├── firmware/
│   └── src/                # ESP32 source code (PID, sensors, UI, BT)
├── docs/
│   ├── DOE_protocol.docx   # Validation protocol (Design of Experiments)
│   └── verification/       # Verification documents
└── README.md
```

*(Adjust this structure to match your actual repository layout.)*

## Validation

The project includes a formal **Design of Experiments (DOE)** protocol for control system validation, along with verification documents in Word format, following a defined institutional format.

## Known Issues / Work in Progress

- **Thermocouple noise**: caused by shared power and ground between the control stage and the power stage. Currently mitigated through software filtering strategies (averaging, digital filters); a definitive fix requires separating the ground planes and/or galvanic isolation in a future PCB revision.

## Suggested Next Steps

- [ ] Physically separate control and power grounds in the next PCB revision.
- [ ] Evaluate optical or digital isolation between stages.
- [ ] Document DOE validation results in this repository.

## Author

Felipe — Mechatronics Engineering, Universidad Nacional de Colombia.

## License
