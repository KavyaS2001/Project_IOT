#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(32,16,2);


int startButton = 8;
int engine = 10;
int engineStatus = 0;
int indicatorLed = 9;
int buttonNew;
int buttonOld = 1;
bool alcoholDetected = false;
int sensorInitialTimeCount = 0;

int engineTerminatingTime = 10;
int sensorTimeCount = 3;


void setup() {
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight(); 

  pinMode(indicatorLed, OUTPUT);
  pinMode(engine, OUTPUT);
  pinMode(startButton, INPUT_PULLUP);  // set the internal pull up resistor, unpressed button is HIGH
  pinMode(12, OUTPUT);
  pinMode(13, INPUT);

  lcd.setCursor(4, 0);
  lcd.print("Welcome");

  lcd.setCursor(1, 1);
  lcd.print("Ready to start");
}

void loop() {
  int gasValue = analogRead(0);  // alcohol sensor


  if (gasValue > 200) {
    digitalWrite(indicatorLed, HIGH);
    sensorInitialTimeCount++;

    if (sensorInitialTimeCount >= (sensorTimeCount * 5)) {
      tone(12, 1000);

      if (engineStatus == 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Alcohol Detected");

        delay(2000);

        lcd.setCursor(0, 0);
        lcd.print("Engine Starting");
        lcd.setCursor(0, 1);
        lcd.print("Disengaged");

        delay(2000);

      } else {
        for (int i = 0; i < 6; i++) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Alcohol Detected");
          delay(1000);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Stop the Vehical");
          lcd.setCursor(2, 1);
          lcd.print("Immediately");
          delay(1000);
        }

        for (int i = engineTerminatingTime; i >= 0; i--) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Engine Terminate");
          lcd.setCursor(0, 1);
          lcd.print("in ");
          lcd.setCursor(4, 1);
          lcd.print(i);
          delay(1000);
        }

        digitalWrite(engine, 0);
        engineStatus = 0;
      }

      alcoholDetected = true;
    }

  } else {
    sensorInitialTimeCount = 0;
    digitalWrite(indicatorLed, LOW);
    noTone(12);

    if (alcoholDetected) {
      sensorInitialTimeCount = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("System Preparing");
      delay(500);
      lcd.setCursor(0, 1);
      for (int i = 0; i < 17; i++) {
        lcd.print(".");
        delay(150);
      }
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Ready to start");
      alcoholDetected = false;
    }

    buttonNew = digitalRead(startButton);
    if (buttonOld == 0 && buttonNew == 1) {
      if (engineStatus == 0) {

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Starting");

        for (int i = 0; i < 3; i++) {
          lcd.print(".");
          delay(1000);
        }

        digitalWrite(engine, 1);
        engineStatus = 1;

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Engine Started");

      } else {
        digitalWrite(engine, 0);
        engineStatus = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Engine");
        lcd.setCursor(0, 1);
        lcd.print("Turning off");
        delay(3000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Ready to start");
      }
    }
    buttonOld = buttonNew;
  }

  delay(200);
}