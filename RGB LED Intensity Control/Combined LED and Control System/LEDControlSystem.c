#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>

// Definition of button pins
#define BUTTON_TOP    (1 << PD2) // Top button pin (pin 4 on ATmega328P)
#define BUTTON_BOTTOM (1 << PD3) // Bottom button pin (pin 5 on ATmega328P)
#define BUTTON_RIGHT  (1 << PD4) // Right button pin (pin 6 on ATmega328P)
#define BUTTON_LEFT   (1 << PD5) // Left button pin (pin 11 on ATmega328P)
#define BUTTON_ON_OFF (1 << PD6) // On/Off button pin (pin 12 on ATmega328P)

// Definition of LED pins
#define LED_RED       (1 << PB3) // Red LED pin (pin 17 on ATmega328P)
#define LED_GREEN     (1 << PB2) // Green LED pin (pin 16 on ATmega328P)
#define LED_BLUE      (1 << PB1) // Blue LED pin (pin 15 on ATmega328P)

#define RED   0    // Macro to represent red color with value 0
#define GREEN 1    // Macro to represent green color with value 1
#define BLUE  2    // Macro to represent blue color with value 2

// EEPROM addresses for each variable
#define EEPROM_IS_ON_ADDRESS      0x00
#define EEPROM_COLOR_SELECT_ADDR  0x01
#define EEPROM_RED_SIGN_ADDR      0x02
#define EEPROM_GREEN_SIGN_ADDR    0x03
#define EEPROM_BLUE_SIGN_ADDR     0x04

uint8_t isOn;        // Variable to check if the system is turned on (1) or off (0)
uint8_t colorSelect; // Variable to identify the selected color (0: red, 1: green, 2: blue)
uint8_t redSign;     // Variable for the intensity of the red color (from 0 to 255)
uint8_t greenSign;   // Variable for the intensity of the green color (from 0 to 255)
uint8_t blueSign;    // Variable for the intensity of the blue color (from 0 to 255)

// Function to write values to EEPROM
void writeToEEPROM() {
  eeprom_write_byte((uint8_t*)EEPROM_IS_ON_ADDRESS, isOn);
  eeprom_write_byte((uint8_t*)EEPROM_COLOR_SELECT_ADDR, colorSelect);
  eeprom_write_byte((uint8_t*)EEPROM_RED_SIGN_ADDR, redSign);
  eeprom_write_byte((uint8_t*)EEPROM_GREEN_SIGN_ADDR, greenSign);
  eeprom_write_byte((uint8_t*)EEPROM_BLUE_SIGN_ADDR, blueSign);
}

// Function to read values from EEPROM
void readFromEEPROM() {
  isOn = eeprom_read_byte((uint8_t*)EEPROM_IS_ON_ADDRESS);
  colorSelect = eeprom_read_byte((uint8_t*)EEPROM_COLOR_SELECT_ADDR);
  redSign = eeprom_read_byte((uint8_t*)EEPROM_RED_SIGN_ADDR);
  greenSign = eeprom_read_byte((uint8_t*)EEPROM_GREEN_SIGN_ADDR);
  blueSign = eeprom_read_byte((uint8_t*)EEPROM_BLUE_SIGN_ADDR);
}

// Configure LED pins for PWM mode
void setupPWM() {
  // Configure Timer 1 for PWM mode on channels A and B
  TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10); // (0b10100001)
  TCCR1B |= (1 << CS10);  // Prescaler configuration for Timer 1 (0b00000001)

  // Configure Timer 2 for PWM mode on channel A
  TCCR2A |= (1 << COM2A1) | (1 << WGM20); // (0b10000011)
  TCCR2B |= (1 << CS20);  // Prescaler configuration for Timer 2 (0b00000001)

  ICR1 = 10000; // Set ICR1 to determine PWM frequency (16MHz)
}

// Function that checks if the pull-up button is pressed
_Bool buttonPressed(int button) {
  return !(PIND & button); // Returns 1 if PIND & button value is 0
}

int main(void) {
  // Configure pins as input (0b01111100)
  DDRD &= ~(BUTTON_ON_OFF | BUTTON_TOP | BUTTON_BOTTOM | BUTTON_RIGHT | BUTTON_LEFT);

  // Configure pins as output (0b00001110)
  DDRB |= LED_RED | LED_GREEN | LED_BLUE;

  setupPWM(); // Call the function to configure LED PWM mode

  readFromEEPROM(); // Read values from EEPROM, overwriting the variables

  while (1) {
    // Toggle the system state if the BUTTON_ON_OFF button is pressed
    if (buttonPressed(BUTTON_ON_OFF)) {
      isOn = !isOn;
      // Wait until the BUTTON_ON_OFF button is released
      while (buttonPressed(BUTTON_ON_OFF)) _delay_ms(1);
    }

    if (isOn) {
      // Increment the colorSelect variable if BUTTON_RIGHT is pressed and colorSelect is less than 2
      if (buttonPressed(BUTTON_RIGHT) && colorSelect < 2) {
        colorSelect++; // Increment colorSelect
        // Wait until BUTTON_RIGHT button is released
        while (buttonPressed(BUTTON_RIGHT)) _delay_ms(1);
      }

      // Decrement the colorSelect variable if BUTTON_LEFT is pressed and colorSelect is greater than 0
      if (buttonPressed(BUTTON_LEFT) && colorSelect > 0) {
        colorSelect--; // Decrement colorSelect
        // Wait until BUTTON_LEFT button is released
        while (buttonPressed(BUTTON_LEFT)) _delay_ms(1);
      }

      switch (colorSelect) {
        case RED:
          // Increment the intensity of the red color if BUTTON_TOP is pressed and intensity is less than or equal to 255
          if (buttonPressed(BUTTON_TOP) && redSign < 255) {
            redSign++;
          }
          // Decrement the intensity of the red color if BUTTON_BOTTOM is pressed and intensity is greater than or equal to 0
          else if (buttonPressed(BUTTON_BOTTOM) && redSign > 0) {
            redSign--;
          }
          break;

        case GREEN:
          // Increment the intensity of the green color if BUTTON_TOP is pressed and intensity is less than or equal to 255
          if (buttonPressed(BUTTON_TOP) && greenSign < 255) {
            greenSign++;
          }
          // Decrement the intensity of the green color if BUTTON_BOTTOM is pressed and intensity is greater than or equal to 0
          else if (buttonPressed(BUTTON_BOTTOM) && greenSign > 0) {
            greenSign--;
          }
          break;

        case BLUE:
          // Increment the intensity of the blue color if BUTTON_TOP is pressed and intensity is less than or equal to 255
          if (buttonPressed(BUTTON_TOP) && blueSign < 255) {
            blueSign++;
          }
          // Decrement the intensity of the blue color if BUTTON_BOTTOM is pressed and intensity is greater than or equal to 0
          else if (buttonPressed(BUTTON_BOTTOM) && blueSign > 0) {
            blueSign--;
          }
          break;

        default:
          break;
      }

      // Set the timer comparison registers to control the color intensities
      OCR2A = redSign;    // Timer 2 - Red color intensity
      OCR1B = greenSign;  // Timer 1 - Green color intensity
      OCR1A = blueSign;   // Timer 1 - Blue color intensity

    } else {
      OCR2A = 0;  // Timer 2 - Red color intensity
      OCR1B = 0;  // Timer 1 - Green color intensity
      OCR1A = 0;  // Timer 1 - Blue color intensity
    }
    writeToEEPROM(); // Write values to EEPROM
    _delay_ms(10);
  }

  return 0;
}
