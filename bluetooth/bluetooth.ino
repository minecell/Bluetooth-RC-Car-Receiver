#include <Servo.h>
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(0, 1); // RX | TX

char incomming;
String mode;
String value;
bool startOfRead = true;

Servo servo;
int pos = 0;
void setup()
{
  servo.attach(9);
  // pinMode(9, OUTPUT);  // this pin will pull the HC-05 pin 34 (key pin) HIGH to switch module to AT mode
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  
  Serial.begin(4800);
  Serial.println("Go");
  BTSerial.begin(9600);
  TIMSK0=0;
}

void loop()
{
  // Keep reading from HC-05 and send to Arduino Serial Monitor
  if (BTSerial.available()) {
    incomming = (char) BTSerial.read();
    // Mode
    if (startOfRead) {
      mode = incomming;
      // Serial.println("Mode: " + mode);
      startOfRead = false;
      return;
    }
    // End
    if (incomming == '#') {
      Serial.println(mode + value);
      if (mode == "s") {
        if (!servo.attached()) {
          servo.attach(9);
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
      if (mode == "m") {
        servo.detach();
        analogWrite(5, value.toInt());        
      }
      // Do somthing with full value and mode
      mode = "";
      value = "";
      startOfRead = true;
      return;
    }
    // Value
    value += incomming;
    // String(incomming)
    // Serial.println("Current value: " + value + ", Incomming: " + incomming);
  }
}
