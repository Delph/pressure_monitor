#ifndef PINS_H_INCLUDE
#define PINS_H_INCLUDE

// buzzer
const uint8_t PIN_BUZZER = 3;

// BMP sensors
const uint8_t BMP_OUTER_CS = 4;
const uint8_t BMP_INNER_CS = 5;

const uint8_t PIN_SW_SILENCE = 2;
const uint8_t PIN_SW_DISPLAY = 8;
const uint8_t PIN_LED_RED = 6;
const uint8_t PIN_LED_GREEN = 7;

// LCD
const uint8_t PIN_RS = 9;
const uint8_t PIN_ENABLE = 10; // pin 10 must be an output
const uint8_t PIN_D4 = A0;
const uint8_t PIN_D5 = A1;
const uint8_t PIN_D6 = A2;
const uint8_t PIN_D7 = A3;

// A6 and A7 only usable for analog input

/** SPI pins, non-configurable
 *  These aren't referred to in code, they're the hardware SPI pins
 *  they're just here so you know
 */
const uint8_t PIN_MOSI = 11;
const uint8_t PIN_MISO = 12;
const uint8_t PIN_CLK = 13;


#endif
