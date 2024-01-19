#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL  // Clock frequency (16 MHz)
#define BAUD_RATE 9600    // Desired baud rate

#define BUTTON PD2

// Function to initialize USART communication
void USART_Init() {
  UBRR0H = (unsigned char)(F_CPU / (16UL * BAUD_RATE) - 1) >> 8;
  UBRR0L = (unsigned char)(F_CPU / (16UL * BAUD_RATE) - 1);
  UCSR0B = (1 << TXEN0); // Enable transmitter
  UCSR0C = (3 << UCSZ00); // 8-bit data configuration
}

// Function to transmit a hexadecimal value via USART
void USART_TransmitHex(uint8_t data) {
  while (!(UCSR0A & (1 << UDRE0))); // Wait for the transmit buffer to be empty
  char upper_nibble = (data >> 4) & 0x0F;
  char lower_nibble = data & 0x0F;
  UDR0 = (upper_nibble < 10) ? (upper_nibble + '0') : (upper_nibble - 10 + 'A');
  while (!(UCSR0A & (1 << UDRE0))); // Wait for the transmit buffer to be empty
  UDR0 = (lower_nibble < 10) ? (lower_nibble + '0') : (lower_nibble - 10 + 'A');
}

// Function to check if the pull-up button is pressed
_Bool buttonPressed(uint8_t button) {
    return !(PIND & (1 << button)); // Returns 1 if PIND & button is 0 (button is pressed)
}

int main(void) {
  // Initialize USART communication
  USART_Init();

  // Configure PD2 as an input (Button pin)
  DDRD &= ~(1 << BUTTON);

  // Initialize command variable
  uint8_t command = 0;

  while (1) {
    // Check if the button is pressed
    if (buttonPressed(BUTTON)) {
      // Increment the command variable
      command++;

      // Reset the command to 0 if it exceeds 3
      if (command > 3) {
        command = 0;
      }

      // Wait for the button to be released
      while (buttonPressed(BUTTON))
        _delay_ms(1);
    }

    // Transmit the corresponding hexadecimal command based on the value of 'command'
    switch (command) {
      case 0:
        // Transmit command 0x00
        USART_TransmitHex(0x00);
        break;

      case 1:
        // Transmit command 0x01
        USART_TransmitHex(0x01);
        break;

      case 2:
        // Transmit command 0x02
        USART_TransmitHex(0x02);
        break;

      case 3:
        // Transmit command 0x03
        USART_TransmitHex(0x03);
        break;

      default:
        break;
    }
  }

  return 0;
}

