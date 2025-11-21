#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define servoPin 3
#define sensorIn 8
#define sensorOut 10
#define buzzerPin 12
#define greenLed 13      // Enter
#define redLed 7         // Exit

Servo gate;
LiquidCrystal_I2C lcd(0x27, 16, 2);

int vehicleCount = 0;
int maxCapacity = 5;
int prevInState = 1;
int prevOutState = 1;

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(sensorIn, INPUT);
  pinMode(sensorOut, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);

  // Loading
  lcd.setCursor(5, 0);
  lcd.print("LOADING");
  lcd.setCursor(0, 1);
  for(int L = 0; L < 16; L++){
    lcd.write(byte(255));
    delay(80);
  }
  delay(1000);

  gate.attach(servoPin);
  gate.write(180);

  // Ready signal
  beepIn(2);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME");
  lcd.setCursor(0, 1);
  lcd.print("COUNT -> 0");
}

void loop() {
  int readIn = digitalRead(sensorIn);
  int readOut = digitalRead(sensorOut);

  // ======== VEHICLE ENTER ========
  if (readIn == LOW && prevInState == HIGH && vehicleCount < maxCapacity) {
    vehicleCount++;

    lcd.setCursor(0, 0);
    lcd.print("WELCOME          ");
    lcd.setCursor(0, 1);
    lcd.print("COUNT -> ");
    lcd.print(vehicleCount);

    beepIn(3);      // green LED
    openGate();
  }
  prevInState = readIn;


  // ======== VEHICLE EXIT ========
  if (readOut == LOW && prevOutState == HIGH && vehicleCount > 0) {
    vehicleCount--;

    lcd.setCursor(0, 0);
    lcd.print("GOODBYE          ");
    lcd.setCursor(0, 1);
    lcd.print("COUNT -> ");
    lcd.print(vehicleCount);

    beepOut(2);     // red LED
    openGate();
  }
  prevOutState = readOut;


  // ======== PARKING INFO ========
  if (vehicleCount == 0) {
    lcd.setCursor(0, 0);
    lcd.print("PARKING INFO    ");
    lcd.setCursor(0, 1);
    lcd.print("COUNT -> 0   ");
  }

  // ======== FULL ========
  else if (vehicleCount == maxCapacity) {
    lcd.setCursor(0, 0);
    lcd.print("PARKING FULL    ");
    lcd.setCursor(0, 1);
    lcd.print("COUNT -> ");
    lcd.print(vehicleCount);

    if (readIn == LOW) {
      beepOut(1);   // red LED warning
    }
  }
}

// ======================= FUNCTIONS =======================

// Beep for ENTER (green LED)
void beepIn(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(greenLed, HIGH);
    delay(200);

    digitalWrite(buzzerPin, LOW);
    digitalWrite(greenLed, LOW);
    delay(200);
  }
}

// Beep for EXIT (red LED)
void beepOut(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(redLed, HIGH);
    delay(200);

    digitalWrite(buzzerPin, LOW);
    digitalWrite(redLed, LOW);
    delay(200);
  }
}

// Servo movement
void openGate() {
  gate.write(120);
  delay(2000);
  gate.write(180);
}
