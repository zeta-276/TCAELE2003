#include <LiquidCrystal_I2C.h>
#include <ACS712.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int EN = 5;
int IN1 = 6;
int IN2 = 7;
bool dir = true;

int but = 4;
int butdebounce = 50;
unsigned long lastdebounce;
bool currentbut = true;

int pot = A1;
int motorSpeed;
int meter = A0;

// 20A ACS712 = 100 mV/A
ACS712 ACS(meter, 5.0, 1023, 100);

unsigned long prev = 0;

void setup() {
  Serial.begin(115200);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(but, INPUT_PULLUP);
  pinMode(EN, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Calibrating");
  ACS.autoMidPoint();
  delay(1000);
  lcd.clear();
}
void loop() {
  unsigned long current = millis();
  currentbut = digitalRead(but);
  if (currentbut == false && (current - lastdebounce) >= butdebounce) {
    lastdebounce = current;
    dir = !dir;
  }
  motorSpeed = map(analogRead(pot), 0, 1023, 0, 255);

  if (dir == true) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }
  else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
  analogWrite(EN, motorSpeed);

  float mA = 0;
  mA = ACS.mA_DC(1);
  mA += 340;

  if (abs(mA) < 50) {
    mA = 0;
  }  

  if (current - prev >= 250) {
    prev = current;
    Serial.println(mA, 3);

    lcd.setCursor(0,0);
    lcd.print("I: ");
    if (mA < 1000) {
    lcd.print(mA, 0);
    lcd.print("mA     ");
    }
    else {
      lcd.print(mA / 1000);
      lcd.print("A     ");
    }
    lcd.setCursor(0,1);
    lcd.print("SPD: ");
    lcd.print(motorSpeed);
    lcd.print(" ");
    if (dir == true) {
      lcd.print("FWD ");
    }
    else {
      lcd.print("REV ");
    }
  }
}