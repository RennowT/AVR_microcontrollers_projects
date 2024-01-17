#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN PD2     // LED pin (pin 4 on ATmega328p)
#define BUTTON_PIN PB1  // Button pin (pin 9 on ATmega328p)

// Function to check if the pull-up button is pressed
_Bool buttonPressed(uint8_t button) {
  return !(PINB & (1 << button)); // Returns 1 if PINB & button is 0 (button is pressed)
}

// Function to control LED flashing pattern
void ledFlashingPattern(uint16_t highTime, uint16_t lowTime) {
  PORTD |= (1 << LED_PIN);      // Turn on the LED
  _delay_ms(highTime);
  PORTD &= ~(1 << LED_PIN);     // Turn off the LED
  _delay_ms(lowTime);
}

int main(void) {
  DDRD |= (1 << LED_PIN);      // Set the pin as output (0b00000100)
  DDRB &= ~(1 << BUTTON_PIN);  // Set the pin as input (0b00000010)

  uint8_t buttonPattern = 0;

  while (1) {
    // Toggle button pattern when the button is pressed
    if (buttonPressed(BUTTON_PIN)) {
      buttonPattern++;
      // Wait until the button is released
      while (buttonPressed(BUTTON_PIN))
        _delay_ms(1);
    } 

    // Use a switch statement to handle different button patterns
    switch (buttonPattern)
    {
    case 0:
      PORTD &= ~(1 << LED_PIN); // Turn off the LED
      break;
    case 1: 
      ledFlashingPattern(500, 500);  // Pattern 1: 500ms on, 500ms off
      break;
    case 2: 
      ledFlashingPattern(800, 200);  // Pattern 2: 800ms on, 200ms off
      break;
    case 3:
      ledFlashingPattern(200, 200);  // Pattern 3: 200ms on, 200ms off
      break;
    default:
      break;
    }

    _delay_ms(10);
  }

  return 0;
}
