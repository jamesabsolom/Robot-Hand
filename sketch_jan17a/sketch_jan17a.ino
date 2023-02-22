//                    GNU AFFERO GENERAL PUBLIC LICENSE (See LICENSE.txt)
//                      Version 3, 19 November 2007

#include <Servo.h>

// Port declaration for fingers
int portPinky = 5; //james likes pink  things
int portRing = 6;
int portMiddle = 10;
int portIndex = 11;
int portThumb = 9;

// Variable declaration for non object linked variables
int potentialMapReverse = 0;
int gestureState = 0;
bool initialized = false;

// Servo declaration, using the servo library included in arduino
Servo pinkyServo;
Servo ringServo;
Servo middleServo;
Servo indexServo;
Servo thumbServo;

// Class for each finger
class servoFinger {
    // Class Variable declaration for variables to be exclusively used inside the class.
    int portRead;             // The port which the finger input is connected to
    int portWrite;            // The port which the finger control wire is connected to
    int potential;            // The potential taken from the potential divider circuit for each finger
  public:
    float potentialAverage;   // The running average of the last 20 taken potential values
  public:
    int potentialMap;         // The mapped potential value converting the taken value and converting it down to 0 to 155
  public:
    int potentialMin;         // The min potential recorded across the potential divider, gets automatically reset to keep the bounds of the flex sensor automatically set
  public:
    int potentialMax;         // The max potential recorded across the potential divider, gets automatically reset to keep the bounds of the flex sensor automatically set
  public:
    float rotation;           // The value for the amount of rotation for the attached servo to go through
  public:
    float storeRotation;      // Stores the rotation before it gets reversed for reversed servos
    bool reverse;             // A simple bool value determining whether the servo is reversed or not on the hand
    Servo servo;              // Holds the Servo declared in ln 19 to ln 23 which is assigned to the individual servo

    // Class function to update the rotation of a servo manually through a fed value
  public:
    void updateRotationManual(float feedRotation) {
      rotation = feedRotation;
      storeRotation = rotation;
      if (reverse)
      {
        rotation = map(rotation, 0, 180, 180, 0);
      }
    }

    // Class function to update the rotation based on the value read across the potential divider
  public:
    updateRotationInput(int potential) {
      potentialAverage = (((potentialAverage * 19) + potential) / 20);
      if (potentialAverage < potentialMin)
      {
        potentialMin = potentialAverage;
      }
      if (potentialAverage > potentialMax)
      {
        potentialMax = potentialAverage;
      }
      potentialMap = map(potentialAverage, potentialMin, potentialMax, 0, 155);
      rotation = potentialMap;
      storeRotation = rotation;
      if (reverse)
      {
        rotation = map(rotation, 0, 180, 180, 0);
      }
    }

    // Class function that makes the servo actually rotate
  public:
    void servoRotate() {
      servo.write(rotation);
    }

    // Class function that sets up all the variables inside the class, essentially just a big feed line to give the variables values
  public:
    void initialize(int feedPortRead, int feedPortWrite, int feedPotentialMin, int feedPotentialMax, Servo feedServo, bool feedReverse) {
      portRead = feedPortRead;
      portWrite = feedPortWrite;
      potentialMin = feedPotentialMin;
      potentialMax = feedPotentialMax;
      servo = feedServo;
      reverse = feedReverse;
      servo.attach(portWrite); //minValue, maxValue);
      potentialAverage = analogRead(portRead);
    }

    // Class function to read the potential divider value without adjusting the servo to match
  public:
    void readFlexSensor() {
      potential = analogRead(portRead);
      potentialAverage = (((potentialAverage * 19) + potential) / 20);
      if (potentialAverage < potentialMin)
      {
        potentialMin = potentialAverage;
      }
      if (potentialAverage > potentialMax)
      {
        potentialMax = potentialAverage;
      }
      potentialMap = map(potentialAverage, potentialMin, potentialMax, 0, 155);
    }

    // Class function to reset the min and max value across the potential divider, used exclusively during testing
  public:
    void resetMinMax() {
      potentialMin = 1023;
      potentialMax = 0;
    }
};

// Declaration of the five finger objects declare00d under the class "servoFinger"
servoFinger pinky;
servoFinger ring;
servoFinger middle;
servoFinger index;
servoFinger thumb;

// Fuction to setup all the "servoFinger" objects through initialization and also cycles through the calibration cycle allowing the player to check the hand is functioning correctly
void setup() {
  pinky.initialize(1, portPinky, 1023, 0, pinkyServo, false);
  ring.initialize(2, portRing, 1023, 0, ringServo, true);
  middle.initialize(3, portMiddle, 1023, 0, middleServo, false);
  index.initialize(4, portIndex, 1023, 0, indexServo, true);
  thumb.initialize(5, portThumb, 1023, 0, thumbServo, true);
  randomSeed(analogRead(0));
  Serial.begin(9600);
  //  Serial.println("Starting Serial");
  //for (int repeates = 0; repeates < 2; repeates++) {
  //  calibrationcycle();
  //}
  moveAllFingers(0);
}

// Movement function to bend all the fingers and then unbend them one by one to check the hand is functioning correctly
void calibrationcycle() {
  pinky.updateRotationManual(0);
  pinky.servoRotate();
  pinky.readFlexSensor();
  delay(500);
  ring.updateRotationManual(0);
  ring.servoRotate();
  ring.readFlexSensor();
  delay(500);
  middle.updateRotationManual(0);
  middle.servoRotate();
  middle.readFlexSensor();
  delay(500);
  index.updateRotationManual(0);
  index.servoRotate();
  index.readFlexSensor();
  delay(500);
  thumb.updateRotationManual(0);
  thumb.servoRotate();
  delay(500);
  pinky.updateRotationManual(155);
  pinky.servoRotate();
  pinky.readFlexSensor();
  delay(500);
  ring.updateRotationManual(155);
  ring.servoRotate();
  ring.readFlexSensor();
  delay(500);
  middle.updateRotationManual(155);
  middle.servoRotate();
  middle.readFlexSensor();
  delay(500);
  index.updateRotationManual(155);
  index.servoRotate();
  index.readFlexSensor();
  delay(500);
  thumb.updateRotationManual(155);
  thumb.servoRotate();
  thumb.readFlexSensor();
  delay(500);
}

// Movement function to reset all fingers to an unbent position
void moveAllFingers(int feedValue) {
  pinky.updateRotationManual(feedValue);
  pinky.servoRotate();
  ring.updateRotationManual(feedValue);
  ring.servoRotate();
  middle.updateRotationManual(feedValue);
  middle.servoRotate();
  index.updateRotationManual(feedValue);
  index.servoRotate();
  thumb.updateRotationManual(feedValue);
  thumb.servoRotate();
}

void randomfingers(servoFinger &feedServoFinger) {
  int randomInt = random(0, 2);
  if (randomInt == 1) {
    feedServoFinger.updateRotationManual(155);
    feedServoFinger.servoRotate();
  }
  else {
    feedServoFinger.updateRotationManual(0);
    feedServoFinger.servoRotate();
  }
}

void controlloop() {

  while (Serial.available()) {
    String input1 = Serial.readStringUntil(',');
    String input2 = Serial.readStringUntil(',');
    String input3 = Serial.readStringUntil(',');
    String input4 = Serial.readStringUntil(',');
    String input5 = Serial.readStringUntil(',');

    pinky.updateRotationInput(input1.toInt());
    pinky.servoRotate();
//    Serial.print(input1);
//    Serial.print(",");
    ring.updateRotationInput(input2.toInt());
    ring.servoRotate();
//    Serial.print(input2);
//    Serial.print(",");
    middle.updateRotationInput(input3.toInt());
    middle.servoRotate();
//    Serial.print(input3);
//    Serial.print(",");
    index.updateRotationInput(input4.toInt());
    index.servoRotate();
//    Serial.print(input4);
//    Serial.print(",");
    thumb.updateRotationInput(input5.toInt());
    thumb.servoRotate();
//    Serial.println(input5);
    Serial.println("recieved");
  }
}

void loop() {
  controlloop();
}
