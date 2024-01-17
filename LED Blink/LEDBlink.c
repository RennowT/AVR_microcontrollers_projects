#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN (1 << PD2)    // LED pin (pin 4 on atmega328p)
#define BUTTON_PIN (1 << PB1) // Button pin (pin 9 on atmega328p)

// Function to check if the pull-up button is pressed
_Bool buttonPressed(uint8_t button) {
  return !(PINB & button); // Returns 1 if PINB & button is 0 (button is pressed)
}

// Function to control LED flashing pattern
void ledFlashingPattern(uint16_t highTime, uint16_t lowTime) {
  PORTD |= LED_PIN;      // Turn on the LED
  _delay_ms(highTime);
  PORTD &= ~LED_PIN;     // Turn off the LED
  _delay_ms(lowTime);
}

int main(void) {
  DDRD |= LED_PIN;      // Set the pin as output (0b00000100)
  DDRB &= ~BUTTON_PIN;  // Set the pin as input (0b00000010)

  _Bool buttonPattern = 0;

  while (1) {
    // Toggle button pattern when the button is pressed
    if (buttonPressed(BUTTON_PIN)) {
      buttonPattern = !buttonPattern;
      
      // Wait until the button is released
      while (buttonPressed(BUTTON_PIN))
        _delay_ms(1);
    } 

    // If the button pattern is active, use the flashing pattern; otherwise, turn off the LED
    if (buttonPattern) {
      ledFlashingPattern(500, 500); // Pattern when the button is pressed
    } else {
      PORTD &= ~LED_PIN; // Turn off the LED
    }

    _delay_ms(10);
  }

  return 0;
}
