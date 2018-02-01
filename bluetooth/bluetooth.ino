#include <Servo.h>
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(0, 1); // RX | TX

const int pMotorSpeed = 5;  // Motor Controller speed
const int pMotorDir1 = 6; // Motor Controller direction 1
const int pMotorDir2 = 7; // Motor Controller direction 2
const int pServo = 9; // Servo rotation
const int pLedWhite = 12; // White Led
const int pLedRed = 13; // Red Led

Servo servo; // servo
char incomming; // incomming char from HC-05(Bluetooth)
String mode; // mode of bluetooth message
String value; // value of bluetooth value
bool startOfRead = true;
int ledState = 1;

void setup()
{
  // Pins als Output definieren und Leds einschalten
  pinMode(pMotorSpeed, OUTPUT);

  pinMode(pMotorDir1, OUTPUT);
  digitalWrite(pMotorDir1, LOW);
  pinMode(pMotorDir2, OUTPUT);
  digitalWrite(pMotorDir2, HIGH);

  servo.attach(pServo);

  pinMode(pLedWhite, OUTPUT);
  digitalWrite(pLedWhite, HIGH);
  pinMode(pLedRed, OUTPUT);
  digitalWrite(pLedRed, HIGH);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Baud Rate festlegen
  Serial.begin(4800);
  // Serial.println("Go");
  BTSerial.begin(9600);
  // Timer Einstellungen ändern
  TIMSK0 = 0;
}

void loop()
{
  // Daten von HC-05 lesen
  if (BTSerial.available()) {
    incomming = (char) BTSerial.read();
    // Erstes Zeichen des Strings definiert für was der Rest verwendet wird (Mode)
    if (startOfRead) {
      mode = incomming;
      // Serial.println("Mode: " + mode);
      startOfRead = false;
      return;
    }
    // '#' ist das letzte Zeichen des Strings
    if (incomming == '#') {
      // Serial.println(mode + value);
      // Servo steuern
      if (mode == "s") {
        if (!servo.attached()) {
          servo.attach(pServo);
        }
        servo.attach(9);
        switch (value.toInt()) {
          case 0: servo.write(45); break;
          case 1: servo.write(90); break;
          case 2: servo.write(135); break;
          default: break;
        }
        // servo.detach();
      }
      // Geschwindigkeit regeln
      if (mode == "m") {
        servo.detach();
        analogWrite(pMotorSpeed, value.toInt());
      }
      // Leds ein- oder ausschalten
      if (mode == "l") { // letter L in lowercase
        servo.detach();
        if (ledState == 1) {
          digitalWrite(pLedWhite, LOW);
          digitalWrite(pLedRed, LOW);
          digitalWrite(LED_BUILTIN, LOW);
          ledState = 0;
        } else {
          digitalWrite(pLedWhite, HIGH);
          digitalWrite(pLedRed, HIGH);
          digitalWrite(LED_BUILTIN, HIGH);
          ledState = 1;
        }
      }
      // Reset
      mode = "";
      value = "";
      startOfRead = true;
      return;
    }
    // alles zwischen dem ersten Zeichen(Mode) und dem letzen Zeichen ('#') wird an value angehänkt 
    value += incomming;
    // String(incomming)
    // Serial.println("Current value: " + value + ", Incomming: " + incomming);
  }
}
