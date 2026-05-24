#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ================= I2C LCD =================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ================= SERVOS =================
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// ================= LDR PINS =================
const int ldr1 = A0;
const int ldr2 = A1;
const int ldr3 = A2;
const int ldr4 = A3;

// ================= BUTTONS =================
const int startButton = 6;
const int stopButton  = 7;

// ================= BUZZER =================
const int buzzer = 8;

// ================= RGB LED =================
const int redPin   = 9;
const int greenPin = 10;
const int bluePin  = 11;

// ================= VARIABLES =================
int score = 0;

bool gameRunning = false;

int threshold = 500;

// Hit protection
bool hit1 = false;
bool hit2 = false;
bool hit3 = false;
bool hit4 = false;

// =================================================
// SETUP
// =================================================
void setup()
{
  Serial.begin(9600);

  // Attach servos
  servo1.attach(2);
  servo2.attach(3);
  servo3.attach(4);
  servo4.attach(5);

  // Buttons
  pinMode(startButton, INPUT_PULLUP);
  pinMode(stopButton, INPUT_PULLUP);

  // RGB LED
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Buzzer
  pinMode(buzzer, OUTPUT);

  // LCD
  lcd.init();
  lcd.backlight();

  // Initial servo positions
  resetTargets();

  // Initial RGB color = RED
  setRGB(255, 0, 0);

  // Welcome screen
  lcd.setCursor(0, 0);
  lcd.print("SHOOTEX GAME");

  lcd.setCursor(0, 1);
  lcd.print("PRESS START");
}

// =================================================
// LOOP
// =================================================
void loop()
{
  // ================= START GAME =================
  if (digitalRead(startButton) == LOW && !gameRunning)
  {
    delay(200);

    gameRunning = true;

    score = 0;

    resetTargets();

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("GAME START!");

    // Blue RGB
    setRGB(0, 0, 255);

    // Start sound
    tone(buzzer, 1000, 200);
    delay(250);

    tone(buzzer, 1500, 200);
    delay(250);

    updateLCD();
  }

  // ================= STOP GAME =================
  if (digitalRead(stopButton) == LOW && gameRunning)
  {
    delay(200);

    gameRunning = false;

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("GAME OVER");

    lcd.setCursor(0, 1);
    lcd.print("SCORE:");
    lcd.print(score);

    // Red RGB
    setRGB(255, 0, 0);

    // Game over sound
    tone(buzzer, 400, 700);

    resetTargets();
  }

  // ================= GAME LOGIC =================
  if (gameRunning)
  {
    checkTarget1();
    checkTarget2();
    checkTarget3();
    checkTarget4();
  }
}

// =================================================
// UPDATE LCD
// =================================================
void updateLCD()
{
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("SCORE:");
  lcd.print(score);

  lcd.setCursor(0, 1);
  lcd.print("Shoot Targets");
}

// =================================================
// RESET TARGETS
// =================================================
void resetTargets()
{
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
  servo4.write(0);

  hit1 = false;
  hit2 = false;
  hit3 = false;
  hit4 = false;
}

// =================================================
// RGB FUNCTION
// =================================================
void setRGB(int redValue, int greenValue, int blueValue)
{
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

// =================================================
// TARGET 1
// =================================================
void checkTarget1()
{
  int value = analogRead(ldr1);

  if (value > threshold && hit1 == false)
  {
    hit1 = true;

    targetHitEffect(servo1);

    score++;

    updateLCD();
  }

  if (value < threshold - 100)
  {
    hit1 = false;
  }
}

// =================================================
// TARGET 2
// =================================================
void checkTarget2()
{
  int value = analogRead(ldr2);

  if (value > threshold && hit2 == false)
  {
    hit2 = true;

    targetHitEffect(servo2);

    score++;

    updateLCD();
  }

  if (value < threshold - 100)
  {
    hit2 = false;
  }
}

// =================================================
// TARGET 3
// =================================================
void checkTarget3()
{
  int value = analogRead(ldr3);

  if (value > threshold && hit3 == false)
  {
    hit3 = true;

    targetHitEffect(servo3);

    score++;

    updateLCD();
  }

  if (value < threshold - 100)
  {
    hit3 = false;
  }
}

// =================================================
// TARGET 4
// =================================================
void checkTarget4()
{
  int value = analogRead(ldr4);

  if (value > threshold && hit4 == false)
  {
    hit4 = true;

    targetHitEffect(servo4);

    score++;

    updateLCD();
  }

  if (value < threshold - 100)
  {
    hit4 = false;
  }
}

// =================================================
// TARGET HIT EFFECT
// =================================================
void targetHitEffect(Servo &servoX)
{
  // Green flash
  setRGB(0, 255, 0);

  // Hit sound
  tone(buzzer, 2000, 150);

  // Move servo
  servoX.write(90);

  delay(500);

  servoX.write(0);

  delay(300);

  // Return to blue
  setRGB(0, 0, 255);
}