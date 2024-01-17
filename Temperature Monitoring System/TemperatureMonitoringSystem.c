#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdio.h>

// Definition of LED pins
#define LED_RED    PD2 // Red LED pin (pin 4 of atmega328p)
#define LED_YELLOW PD3 // Yellow LED pin (pin 5 of atmega328p)
#define LED_GREEN  PD4 // Green LED pin (pin 6 of atmega328p)

// Coefficients for the Steinhart-Hart equation
#define A_COEFFICIENT 0.001129148
#define B_COEFFICIENT 0.000234125
#define C_COEFFICIENT 0.0000000876741

#define R2 10000 // Fixed resistor value (in ohms)
#define R0 10000 // Resistance of the thermistor at a reference temperature (in ohms)

void setupADC() {
  // ADC Configuration
  ADMUX |= (1 << REFS0);         // Reference voltage as AVcc
  ADMUX &= ~(1 << ADLAR);        // Right adjust for 10 bits (values from 0 to 1023)
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Clock division factor 128
  ADCSRA |= (1 << ADEN);         // Enable ADC
}

uint16_t readADC0() {
  // Select ADC0 channel (pin A0)
  ADMUX &= ~(1 << MUX0) & ~(1 << MUX1) & ~(1 << MUX2) & ~(1 << MUX3);

  // Start ADC conversion
  ADCSRA |= (1 << ADSC);

  // Wait for conversion to complete
  while (ADCSRA & (1 << ADSC));

  // Return the read value (10-bit ADC)
  return ADC;
}

uint16_t calculateThermistorResistance(float adcReading) {
  // Calculate thermistor resistance using the voltage divider formula
  uint16_t thermistorResistance = R2 * (1023 / adcReading - 1);

  return thermistorResistance;
}

short calculateTemperature(float thermistorResistance) {
  // Calculate temperature in Kelvin using the Steinhart-Hart equation
  short kelvin = 1 / (A_COEFFICIENT + B_COEFFICIENT * log(thermistorResistance) + C_COEFFICIENT * pow(log(thermistorResistance), 3));

  // Convert to Celsius
  return kelvin - 273;
}

void initializeSerial() {
  // Set baud rate to 9600 bps
  UBRR0H = (unsigned char)(103 >> 8);
  UBRR0L = (unsigned char)103;

  // Enable transmission and reception
  UCSR0B = (1 << TXEN0) | (1 << RXEN0);

  // Set frame format (8 data bits, 1 stop bit)
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void transmitSerial(char data) {
  // Wait until the transmission register is empty
  while (!(UCSR0A & (1 << UDRE0)));

  // Put the data into the transmission register
  UDR0 = data;
}

// Convert a floating-point number to string
void ftoa(float num, char* buffer) {
  int integerPart = (int)num;

  sprintf(buffer, "%d", integerPart);
}

char buffer[6];  // Buffer to store the formatted string

int main(void) {
  setupADC();
  initializeSerial();

  DDRD |= (1 << LED_GREEN);
  DDRD |= (1 << LED_YELLOW);
  DDRD |= (1 << LED_RED);

  while (1) {
    uint16_t adcReading = readADC0();
    uint16_t thermistorResistance = calculateThermistorResistance(adcReading);
    short temperatureCelsius = calculateTemperature(thermistorResistance);
    
    // LED control based on temperature
    if (temperatureCelsius <= 25) {
      PORTD |= (1 << LED_GREEN);
      PORTD &= ~(1 << LED_YELLOW);
      PORTD &= ~(1 << LED_RED);
    } else if (temperatureCelsius > 25 && temperatureCelsius <= 35) {
      PORTD &= ~(1 << LED_GREEN);
      PORTD |= (1 << LED_YELLOW);
      PORTD &= ~(1 << LED_RED);
    } else {
      PORTD &= ~(1 << LED_GREEN);
      PORTD &= ~(1 << LED_YELLOW);
      PORTD |= (1 << LED_RED);
    }

    // Convert values to strings
    char temperatureCelsiusStr[2];
    ftoa(temperatureCelsius, temperatureCelsiusStr);

    // Send data through serial port
    transmitSerial('T');
    transmitSerial(':');
    for (uint8_t i = 0; temperatureCelsiusStr[i] != '\0'; i++) {
      transmitSerial(temperatureCelsiusStr[i]);
    }

    transmitSerial('\r');
    transmitSerial('\n');

    // Wait for some time
    _delay_ms(1000);
  }

  return 0;
}
