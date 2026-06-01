#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
int trig=2;
int echo=3;
float distancecm;
float distancein;
long duration;
int del1=2;
int del2=10;

// character creation for the LCD
byte infinityLeft[8] = {
  B00000,
  B01110,
  B10001,
  B10000,
  B10001,
  B01110,
  B00000,
  B00000
};

byte infinityRight[8] = {
  B00000,
  B01110,
  B10001,
  B00001,
  B10001,
  B01110,
  B00000,
  B00000
};

// setup
void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // assign new characters for the LCD
  lcd.createChar(0, infinityLeft);
  lcd.createChar(1, infinityRight);
}
void loop() {
  digitalWrite(trig, LOW);
  delayMicroseconds(del1);
  digitalWrite(trig, HIGH);
  delayMicroseconds(del2);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH, 20000);    // timeout after 20ms if no echo is recieved

  if (duration == 0) {
    distancecm = 999;
    distancein = 999;
  }

  distancecm = duration * 0.01715;    // distance in centimetres
  distancein = duration * 0.006755;   // distance in inches

  // print infinity if duration timed out
  if (distancecm >= 999) {
    lcd.setCursor(0,0);
    lcd.print("D in cm: ");
    lcd.write(byte(0));
    lcd.write(byte(1));
    lcd.setCursor(0,1);
    lcd.print("D in inch: ");
    lcd.write(byte(0));
    lcd.write(byte(1));
  }  
  // print distance in centimetres and inches
  else {
    lcd.setCursor(0,0);
    lcd.print("D in cm: ");
    lcd.print(distancecm);
    lcd.setCursor(0,1);
    lcd.print("D in inch: ");
    lcd.print(distancein);
  }
}