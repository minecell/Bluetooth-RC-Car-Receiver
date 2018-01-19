#include <SoftwareSerial.h>

SoftwareSerial BTSerial(0, 1); // RX | TX

char incomming;
String mode;
String value;
bool startOfRead = true;
void setup()
{
  pinMode(9, OUTPUT);  // this pin will pull the HC-05 pin 34 (key pin) HIGH to switch module to AT mode
  digitalWrite(9, HIGH);
  Serial.begin(4800);
  Serial.println("Go");
  BTSerial.begin(9600);
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
