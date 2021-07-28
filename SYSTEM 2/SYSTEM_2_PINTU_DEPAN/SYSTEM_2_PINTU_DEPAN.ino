
#include <Servo.h>

// constants won't change
const int TOUCH_SENSOR_PIN = 10; // D8Arduino pin connected to touch sensor's pin
const int SERVO_PIN        = 9; //D7 Arduino pin connected to servo motor's pin

Servo servo; // create servo object to control a servo

// variables will change:
int angle = 0;         // the current angle of servo motor
int lastTouchState;    // the previous state of touch sensor
int currentTouchState; // the current state of touch sensor

void setup() {
  Serial.begin(9600);               // initialize serial
  pinMode(TOUCH_SENSOR_PIN, INPUT); // set arduino pin to input mode
  servo.attach(SERVO_PIN);          // attaches the servo on pin 9 to the servo object

  servo.write(angle);
  currentTouchState = digitalRead(TOUCH_SENSOR_PIN);
}

void loop() {
  lastTouchState    = currentTouchState;             // save the last state
  currentTouchState = digitalRead(TOUCH_SENSOR_PIN); // read new state

  if(lastTouchState == LOW && currentTouchState == HIGH) {
    Serial.println("The sensor is touched");

    // change angle of servo motor
    if(angle == 0)
      angle = 90;
    else
    if(angle == 90)
      angle = 0;

    // control servo motor arccoding to the angle
    servo.write(angle);
  }

}
