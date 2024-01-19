#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL  // Clock frequency (16 MHz)
#define BAUD_RATE 9600    // Desired baud rate

#define LED_GREN PB2
#define LED_RED PB1

// Function to initialize USART communication
void USART_Init() {
  UBRR0H = (unsigned char)(F_CPU / (16UL * BAUD_RATE) - 1) >> 8;
  UBRR0L = (unsigned char)(F_CPU / (16UL * BAUD_RATE) - 1);
  UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Enable receiver and transmitter
  UCSR0C = (3 << UCSZ00); // 8-bit data configuration
}

// Function to receive a hexadecimal value via USART
char USART_ReceiveHex() {
  while (!(UCSR0A & (1 << RXC0))); // Wait for reception of data
  char upper_nibble = UDR0;
  while (!(UCSR0A & (1 << RXC0))); // Wait for reception of data
  char lower_nibble = UDR0;

  // Convert hexadecimal characters to values from 0 to 15
  upper_nibble -= (upper_nibble >= 'A') ? ('A' - 10) : '0';
  lower_nibble -= (lower_nibble >= 'A') ? ('A' - 10) : '0';

  // Explanation:
  // If upper_nibble or lower_nibble is between '0' and '9', subtract '0' to get the corresponding numeric value.
  // If between 'A' and 'F', subtract 'A' - 10 to convert 'A' to 'F' to numeric values from 10 to 15.

  // Combine half-bytes to form a complete byte
  return (upper_nibble << 4) | lower_nibble;

  // Explanation:
  // The four most significant bits (MSB) of upper_nibble are shifted to the left by 4 positions
  // using the left shift operator (<< 4). Then, these bits are combined (|) with the
  // four least significant bits (LSB) of lower_nibble to form a complete byte.
}

int main(void) {
  // Initialize USART communication
  USART_Init();

  // Configure PB2 and PB1 as outputs (LEDs)
  DDRB |= (1 << LED_GREN);
  DDRB |= (1 << LED_RED);

  while (1) {
    // Receive a hexadecimal value from USART
    char receivedValue = USART_ReceiveHex();

    // Perform actions based on the received value
    if (receivedValue == 0x00) {
      // Turn on the green LED and turn off the red LED
      PORTB |= (1 << LED_GREN);
      PORTB &= ~(1 << LED_RED);
    } else if (receivedValue == 0x01){
      // Turn on the red LED and turn off the green LED
      PORTB |= (1 << LED_RED);
      PORTB &= ~(1 << LED_GREN);
    } else if (receivedValue == 0x02){
      // Turn on both the green and red LEDs
      PORTB |= (1 << LED_GREN) | (1 << LED_RED);
    } else if (receivedValue == 0x03){
      // Turn off both the green and red LEDs
      PORTB &= ~((1 << LED_GREN) | (1 << LED_RED));
    } 
  }

  return 0;
}
