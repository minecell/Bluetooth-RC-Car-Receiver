#include <SoftwareSerial.h>
#include <Servo.h>
SoftwareSerial BTSerial(0, 1); // RX | TX
Servo servo;
char incomingByte;
String mode;
String value;
bool modeFound = false;
int servoTarget = 90;
int servoRate = 1;
void setup()
{
  // Motor Controller
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  // Set initial rotation direction
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);

  // Bluetooth
  pinMode(9, OUTPUT);  // this pin will pull the HC-05 pin 34 (key pin) HIGH to switch module to AT mode
  digitalWrite(9, HIGH);
  // Servo
  servo.attach(4); // 14 == A0
  // Serial
  Serial.begin(4800);
  Serial.println("Gott zum Gruß Ihr edler Herr und stolzer Ritter aus edlem Geschlecht! Es mir eine Freude, Euch hier zu erblicken. ");
  // BT Serial
  BTSerial.begin(9600);  // 38400 HC-05 default speed in AT command more
  // Center servo
  servo.write(servoTarget);
}

void loop()
{
  /*
  if (Serial.available() > 0) {
    // read data
    value = Serial.readString();
    Serial.println(value);
    analogWrite(5, value.toInt());
  }
  */
  bluetooth();
  controlServo();
}

void controlServo() {
  if(servo.read() > servoTarget){
      servo.write(servo.read() - servoRate);
    } else if(servo.read() < servoTarget){
      servo.write(servo.read() + servoRate);
    }
}

void bluetooth() {
  if (BTSerial.available()) {
    // read data
    incomingByte = BTSerial.read();
    // get first byte, it decides purpose of "value"
    if (!modeFound) {
      mode = incomingByte;
      modeFound = true;
      return;
    }
    // get last byte, do somthing with "mode" and "value"
    if (incomingByte == '#') {
      // Print
      // Serial.println(mode + value);
      // (s)teering
      if (mode == "s") {
        switch (value.toInt()) {
          case 0: servoTarget = 45; break;
          case 1: servoTarget = 90; break;
          case 2: servoTarget = 135; break;
          default: servoTarget = 90;
        }

        // servoTarget = value.toInt();
        // servo.write(value.toInt());
      }
      // (m)otor
      if (mode == "m") {
        // Serial.println("Mode m == " + mode);
        analogWrite(5, value.toInt());
      }
      // reset
      mode = "";
      value = "";
      modeFound = false;
      return;
    }
    // all other bytes will be added to value
    value += incomingByte;
  }
}
