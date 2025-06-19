
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// LCD I2C setup
LiquidCrystal_I2C lcd(0x27, 16, 2);
// IR sensor pins
#define SENSOR_1_PIN 2 // INT0
#define SENSOR_2_PIN 3 // INT1
// Variables
volatile unsigned long timeSensor1 = 0;
volatile unsigned long timeSensor2 = 0;
volatile bool sensor1Triggered = false;
volatile bool sensor2Triggered = false;
int inCount = 0;
int outCount = 0;
const float distanceBetweenSensors = 0.1; // meters
float speed = 0; // m/s
void setup() {
lcd.init();
lcd.backlight();
lcd.setCursor(0, 0);
lcd.print("System Starting...");
delay(1500);
lcd.clear();
pinMode(SENSOR_1_PIN, INPUT);
pinMode(SENSOR_2_PIN, INPUT);
attachInterrupt(digitalPinToInterrupt(SENSOR_1_PIN), sensor1ISR, RISING);
attachInterrupt(digitalPinToInterrupt(SENSOR_2_PIN), sensor2ISR, RISING);
Serial.begin(9600);
}
void loop() {
if (sensor1Triggered && sensor2Triggered) {
sensor1Triggered = false;
sensor2Triggered = false;
long timeDiff = timeSensor2 - timeSensor1;
speed = 0;
if (timeDiff > 0) {
// Direction: IN (Sensor1 → Sensor2)
speed = distanceBetweenSensors / (timeDiff / 1000000.0);
inCount++;
} else if (timeDiff < 0) {
// Direction: OUT (Sensor2 → Sensor1)
speed = distanceBetweenSensors / ((-timeDiff) / 1000000.0);
outCount++;
}
// Display on LCD
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("IN:");
lcd.print(inCount);
lcd.print(" OUT:");
lcd.print(outCount);
lcd.setCursor(0, 1);
lcd.print("Speed:");
lcd.print(speed, 2);
lcd.print("m/s");
// Debug
Serial.print("IN: ");
Serial.print(inCount);
Serial.print(" | OUT: ");
Serial.print(outCount);
Serial.print(" | Speed: ");
Serial.print(speed, 2);
Serial.println(" m/s");
delay(500); // Short pause for readability
}
}
void sensor1ISR() {
if (!sensor1Triggered) {
timeSensor1 = micros();
sensor1Triggered = true;
}
}
void sensor2ISR() {
if (!sensor2Triggered) {
timeSensor2 = micros();
sensor2Triggered = true;
}
}
