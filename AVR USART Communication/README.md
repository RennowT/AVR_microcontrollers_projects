# AVR USART Communication Project

This directory contains the source code for a simple AVR project implementing USART communication between a transmitter and a receiver using ATmega328P microcontrollers.

## Overview

The project consists of two main components:

1. **Transmitter (TX):**
   - Increments a command value on button press.
   - Transmits hexadecimal commands (0x00 to 0x03) through USART based on the button presses.
   - Configurable baud rate.

2. **Receiver (RX):**
   - Initializes USART communication.
   - Receives hexadecimal commands from the transmitter.
   - Controls LEDs based on received commands.

## Requirements

- AVR-GCC toolchain
- AVRDUDE for programming the microcontrollers
- AVR Microcontrollers (ATmega328P used in this project)
- AVR Programmer (e.g., USBasp)
- Development environment (e.g., Atmel Studio, VSCode with PlatformIO)

## How to Use

1. Connect the transmitter and receiver microcontrollers to the respective hardware.
2. Compile and flash the code onto the microcontrollers.
3. Ensure proper connections between transmitter and receiver (e.g., USART connections).
4. Power up the system.
5. Observe the LEDs on the receiver based on the button presses on the transmitter.

## Transmitter (TX) Code Explanation

- Initializes USART communication and sends hexadecimal commands based on button presses.

## Receiver (RX) Code Explanation

- Initializes USART communication and controls LEDs based on received commands.

## USART Communication

- Communication is established using USART (Universal Synchronous and Asynchronous Receiver-Transmitter).
- Baud rate is configurable in the code.
