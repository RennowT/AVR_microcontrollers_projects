# RGB LED Color Intensity Controller

## Overview
This project is an RGB LED color intensity controller that allows you to adjust the intensity of each color component (red, green, and blue) individually. The system includes buttons for power control, color selection, and intensity adjustments.

## Hardware Requirements
- AVR microcontroller (e.g., ATmega328P)
- RGB LED (common cathode or common anode)
- Resistors for the RGB LED
- Pushbuttons (On/Off, Top, Bottom, Right, Left)
- Protoboard and jumper wires
- Multimeter

## Pin Configuration
- Button Pins:
  - On/Off: PD6
  - Top: PD2
  - Bottom: PD3
  - Right: PD4
  - Left: PD5

- LED Pins:
  - Red: PB3
  - Green: PB2
  - Blue: PB1

## How to Use

1. **Power On/Off:**
   - Press the On/Off button (PD6) to toggle the system state.

2. **Color Selection:**
   - Use the Right (PD4) and Left (PD5) buttons to cycle through available colors (Red, Green, Blue).

3. **Intensity Adjustment:**
   - Use the Top (PD2) and Bottom (PD3) buttons to increase or decrease the intensity of the selected color.

4. **EEPROM Storage:**
   - The system state, color selection, and color intensities are stored in EEPROM to retain settings between power cycles.

5. **PWM Configuration:**
   - Timer 1 and Timer 2 are configured for PWM mode to control the intensity of each color.
