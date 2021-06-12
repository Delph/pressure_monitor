#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <LiquidCrystal.h>

#include "config.h"
#include "debounce.h"
#include "pins.h"


Adafruit_BMP280 bmpInner(BMP_INNER_CS);
Adafruit_BMP280 bmpOuter(BMP_OUTER_CS);

LiquidCrystal lcd(PIN_RS, PIN_ENABLE, PIN_D4, PIN_D5, PIN_D6, PIN_D7);

Debounce sw_display(PIN_SW_DISPLAY);
Debounce sw_silence(PIN_SW_SILENCE);

bool displayTemperature = false;

uint32_t alarm = 0;
bool armed = true;

void setup()
{
  sw_display.init();
  sw_silence.init();

  lcd.begin(16, 2);
  lcd.display();

  pinMode(PIN_LED_RED, OUTPUT);
  digitalWrite(PIN_LED_RED, 0);
  pinMode(PIN_LED_GREEN, OUTPUT);
  digitalWrite(PIN_LED_GREEN, 0);

  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, 0);

  if (!bmpInner.begin())
  {
    lcd.setCursor(0, 0);
    lcd.print("Error:");
    lcd.setCursor(0, 1);
    lcd.print("Chamber sensor");
    while (true);
  }

  if (!bmpOuter.begin())
  {
    lcd.setCursor(0, 0);
    lcd.print("Error:");
    lcd.setCursor(0, 1);
    lcd.print("External sensor");
    while (true);
  }

  bmpInner.setSampling(Adafruit_BMP280::MODE_NORMAL,
    Adafruit_BMP280::SAMPLING_X2,
    Adafruit_BMP280::SAMPLING_X16,
    Adafruit_BMP280::FILTER_X16,
    Adafruit_BMP280::STANDBY_MS_500);

  bmpOuter.setSampling(Adafruit_BMP280::MODE_NORMAL,
    Adafruit_BMP280::SAMPLING_X2,
    Adafruit_BMP280::SAMPLING_X16,
    Adafruit_BMP280::FILTER_X16,
    Adafruit_BMP280::STANDBY_MS_500);
}

float innerPressures[AVERAGE_NUM_READINGS] = {0};
float innerTemperatures[AVERAGE_NUM_READINGS] = {0};
float outerPressures[AVERAGE_NUM_READINGS] = {0};
float outerTemperatures[AVERAGE_NUM_READINGS] = {0};
size_t index = 0;

float average(const float* const items, const size_t len)
{
  float s = 0.0f;

  for (size_t i = 0; i < len; ++i)
    s += items[i];
  return s / len;
}

uint32_t nextUpdate = 0;
void loop()
{
  sw_display.update();
  sw_silence.update();

  if (sw_display.is_released())
  {
    displayTemperature = !displayTemperature;

    // force an update
    nextUpdate = millis();
  }

  if (sw_silence.is_released())
  {
    alarm = 0;
    armed = false;
    digitalWrite(PIN_BUZZER, 0);
  }


  if (alarm)
    digitalWrite(PIN_BUZZER, ((millis() - alarm) / 500) % 2);

  const float innerPressure = bmpInner.readPressure() / 100.0f;
  const float innerTemperature = bmpInner.readTemperature();
  const float outerPressure = bmpInner.readPressure() / 100.0f;
  const float outerTemperature = bmpInner.readTemperature();


  innerPressures[index] = innerPressure;
  innerTemperatures[index] = innerTemperature;
  outerPressures[index] = outerPressure;
  outerTemperatures[index] = outerTemperature;
  index = (index + 1) % AVERAGE_NUM_READINGS;

  if (millis() >= nextUpdate)
  {
    nextUpdate = millis() + UPDATE_INTERVAL;

    float innerPressureAverage = average(innerPressures, AVERAGE_NUM_READINGS);
    float outerPressureAverage = average(outerPressures, AVERAGE_NUM_READINGS);
    float innerTemperatureAverage = average(innerTemperatures, AVERAGE_NUM_READINGS);
    float outerTemperatureAverage = average(outerTemperatures, AVERAGE_NUM_READINGS);

    // red LED on when the chamber pressure is equal or higher
    digitalWrite(PIN_LED_RED, innerPressureAverage >= outerPressureAverage);
    digitalWrite(PIN_LED_GREEN, !(innerPressureAverage >= outerPressureAverage));
    if (armed)
    {
      if (alarm == 0 && innerPressureAverage >= outerPressureAverage)
        alarm = millis();
    }
    else
    {
      if (innerPressureAverage < outerPressureAverage)
        armed = true;
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Cha: ");
    if (!displayTemperature)
    {
      lcd.print(innerPressureAverage);
      lcd.print(" hPa");
    }
    else
    {
      lcd.print(innerTemperatureAverage);
      lcd.print(" *C");
    }
    lcd.setCursor(0, 1);
    lcd.print("Ext: ");
    if (!displayTemperature)
    {
      lcd.print(outerPressureAverage);
      lcd.print(" hPa");
    }
    else
    {
      lcd.print(outerTemperatureAverage);
      lcd.print(" *C");
    }
  }
}
