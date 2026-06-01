#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

int red = 9;
int gre = 10;
int blu = 11;
int but = 5;
int pot = A0;
int potval;
bool manual = false;
int potvalLED;

unsigned long lastLED = 0;
int RGB = 0;
int step = 1;
int steptime = 1;

void setRainbow(int value) {
  int r, g, b;

  if (value <= 255) {
    r = value;
    g = 255 - value;
    b = 255;
  }
  else if (value <= 510) {
    int x = map(value, 256, 510, 0, 255);
    r = 255;
    g = x;
    b = 255 - x;
  }
  else {
    int x = map(value, 511, 765, 0, 255);
    r = 255 - x;
    g = 255;
    b = x;
  }

  analogWrite(red, r);
  analogWrite(gre, g);
  analogWrite(blu, b);
}

void setup() {
  pinMode(red, OUTPUT);
  pinMode(gre, OUTPUT);
  pinMode(blu, OUTPUT);
  pinMode(but, INPUT_PULLUP);
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
}

void loop() {
  if (digitalRead(but) == LOW) {
    manual = !manual;
    delay(300);
  }
  if (manual) {
    potval = map(analogRead(pot), 0, 1023, 0, 765);
    if (potval <= 255) {
      potvalLED = constrain(potval, 0, 255);
      analogWrite(red, potval);
      analogWrite(gre, 255 - potvalLED);
      analogWrite(blu, 255);
    }
    else if (potval > 255 && potval <= 510) {
      potvalLED = map(constrain(potval, 256, 510), 256, 510, 0, 255);
      analogWrite(red, 255);
      analogWrite(gre, potvalLED);
      analogWrite(blu, 255 - potvalLED);
    }
    else if (potval > 510) {
      potvalLED = map(constrain(potval, 511, 765), 511, 765, 0, 255);
      analogWrite(red, 255 - potvalLED);
      analogWrite(gre, 255);
      analogWrite(blu, potvalLED);
    }
  }
  else {
    if (millis() - lastLED >= steptime) {
      lastLED = millis();

      setRainbow(RGB);

      RGB += step;

      if (RGB > 765) {
        RGB = 0;
      }
    }
  }
  lcd.setCursor(0,0);
  lcd.print("MODE: ");
  if (manual) {
    lcd.print("Manual         ");
  }
  else {
    lcd.print("Automatic      ");
  }
  Serial.println(potval);
  Serial.println(potvalLED);
  Serial.println("---");
  Serial.print(digitalRead(but));
  Serial.print(" ");
  Serial.println(manual);
}