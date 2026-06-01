#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int voltage = A0;

const float Vref = 3.3;
const float R1 = 10000.0;
const float R2 = 10000.0;

unsigned long lastLCD = 0;
unsigned long lastSerial = 0;

void setup() {
  analogReadResolution(10);

  lcd.init();
  lcd.backlight();

  Serial.begin(115200);

  lcd.setCursor(0, 0);
  lcd.print("Voltage Meter");
  delay(1000);
  lcd.clear();
}

void loop() {
  int adcValue = analogRead(voltage);

  float Vout = (adcValue / 1023.0) * Vref;
  float Vin = Vout * ((R1 + R2) / R2);

  if (millis() - lastLCD >= 250) {
    lastLCD = millis();

    lcd.setCursor(0, 0);
    lcd.print("ADC: ");
    lcd.print(adcValue);
    lcd.print("     ");

    lcd.setCursor(0, 1);
    lcd.print("Vin: ");
    lcd.print(Vin, 2);
    lcd.print(" V     ");
  }

  if (millis() - lastSerial >= 500) {
    lastSerial = millis();

    Serial.println(Vin);   // MATLAB-friendly clean value
  }
}