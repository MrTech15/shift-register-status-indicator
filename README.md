# shift-register-status-indicator
Reusable SN74HC595 diagnostic indicator module for embedded systems.

# Shift Register Status Indicator Module

A reusable embedded diagnostics indicator built with an Arduino, SN74HC595 shift register, and LEDs.

## Features
- Status + behaviour architecture
- GOOD, OKAY, FAULT, and CLEAR states
- SOLID, SLOW, and FAST visual behaviours
- Non-blocking blink timing using millis()
- Reusable function interface for future projects

## Hardware
Arduino Uno
SN74HC595 shift register
Red, yellow, and green LEDs
220ohm Resistor
Breadboard and jumper wires

## Purpose
This project demonstrates how a simple shift register can be used to create a reusable visual diagnostics module for robotics, automation, and embedded systems.
