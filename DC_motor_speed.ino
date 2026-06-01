#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int EN = 5;
int IN1 = 6;
int IN2 = 7;
bool dir = true;

int but = 4;
int butdebounce = 500;
unsigned long lastdebounce;
bool currentbut = true;

int pot = A1;
int motorSpeed;


unsigned long prev = 0;

void setup() {
  Serial.begin(115200);
  pinMode(IN1, OUTPUT);z
  pinMode(IN2, OUTPUT);
  pinMode(but, INPUT_PULLUP);
  pinMode(EN, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Calibrating");
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

  lcd.setCursor(0,0);
  lcd.print("DC Motor speed:    ");

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