#include <Servo.h>
// Servo declaration, using the servo library included in arduino
Servo pinkyServo;
Servo ringServo;
Servo middleServo;
Servo indexServo;
Servo thumbServo;

int portPinky = 5;
int portRing = 6;
int portMiddle = 10;
int portIndex = 11;
int portThumb = 9;

void setup() {
  Serial.begin(9600);
  pinkyServo.attach(portPinky);
  ringServo.attach(portRing);
  middleServo.attach(portMiddle);
  indexServo.attach(portIndex);
  thumbServo.attach(portThumb);
  pinkyServo.write(0);
  ringServo.write(180);
  middleServo.write(0);
  indexServo.write(180);
  thumbServo.write(180);
}

void loop() {
  int finger1 = 0;
  int finger2 = 0;
  int finger3 = 0;
  int finger4 = 0;
  int finger5 = 0;

  if (Serial.available() > 0) {
    finger1 = Serial.parseInt();
    finger2 = Serial.parseInt();
    finger3 = Serial.parseInt();
    finger4 = Serial.parseInt();
    finger5 = Serial.parseInt();
    Serial.print("Received Value: ");
    Serial.println(finger1);
    Serial.println(finger2);
    Serial.println(finger3);
    Serial.println(finger4);
    Serial.println(finger5);
    Serial.println("Step");
  }
  
  pinkyServo.write(finger1);
  ringServo.write(180 - finger2);
  middleServo.write(finger3);
  indexServo.write(180 - finger4);
  thumbServo.write(180 - finger5);
}
