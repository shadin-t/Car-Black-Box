# Car Black Box System

This project is a vehicle event logging system designed using the PIC18F4580 microcontroller. It mimics the functionality of a black box by continuously monitoring and recording data such as gear shifts, speed, and collision events.

The system integrates multiple peripherals and captures real-time data to help analyze vehicle behavior after incidents. Developed as part of the Embedded Systems training at Emertxe Information Technologies.

## Features

- Real-time vehicle speed monitoring using ADC
- Gear position detection via matrix keypad
- Collision detection simulation
- Event logging with time-stamps using RTC (DS1307)
- Data storage in onboard EEPROM
- LCD display for live feedback
- Serial output via UART for monitoring/debugging

## Technologies Used

- Embedded C
- MPLAB X IDE
- PIC18F4580 Microcontroller
- ADC, EEPROM, RTC, UART, LCD
- Proteus Simulator (for testing)


## How It Works

- Speed is read from a variable resistor (simulating a sensor) using ADC.
- Gear input is taken via a matrix keypad.
- RTC adds time-stamps to each event.
- Data is stored in EEPROM and optionally sent over UART.
- LCD provides feedback for real-time monitoring.

## Learning Outcomes

- Interfaced and programmed multiple hardware peripherals (ADC, EEPROM, RTC, LCD, UART)
- Understood real-time data acquisition and logging
- Applied Embedded C to build a functional, interrupt-driven system
