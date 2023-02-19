#include "barrier.h"
#include "trainDetector.h"

#define CLOSE_NO_TRAIN 0
#define UP_TRAIN_DETECT 1
#define OPEN_TRAIN_DETECT 2
#define OPEN_WAIT_NO_TRAIN 3
#define DOWN_NO_TRAIN 4
#define WAIT_TIMEOUT (6 * 1000)

#define ANGLE_OPEN 0
#define ANGLE_CLOSED 80

const int echoPin = 0;
const int trigPin = 1;
const int servoPin = 7;

// state machine
long waitStarted = 0;
int state = CLOSE_NO_TRAIN;

Barrier barrier(servoPin, ANGLE_CLOSED);
TrainDetector trainDetector(trigPin, echoPin);

void setup() {
  Serial.begin(9600); // Starts the serial communication
}

void setUpTrainDetectState() {
  state = UP_TRAIN_DETECT;
  barrier.setAngle(ANGLE_OPEN);
}

void loop() {
  boolean isTrainDetected = trainDetector.detectTrain();
  switch (state) {
    case CLOSE_NO_TRAIN:
      if (isTrainDetected) {
        setUpTrainDetectState();
      }
      break;
    case UP_TRAIN_DETECT:
      if (barrier.getAngle() == ANGLE_OPEN) {
        state = OPEN_TRAIN_DETECT;
      }
      break;
    case OPEN_TRAIN_DETECT:
      if (!isTrainDetected) {
        state = OPEN_WAIT_NO_TRAIN;
        waitStarted = millis();
      }
      break;
    case OPEN_WAIT_NO_TRAIN:
      if (isTrainDetected) {
        setUpTrainDetectState();
      }
      else if (millis() - waitStarted > WAIT_TIMEOUT) {
        barrier.setAngle(ANGLE_CLOSED);
        state = DOWN_NO_TRAIN;
      }
      break;
    case DOWN_NO_TRAIN:
      if (isTrainDetected) {
        setUpTrainDetectState();
      }
      else if (barrier.getAngle() == ANGLE_CLOSED) {
        state = CLOSE_NO_TRAIN;
      }
      break;
    default:
      break;
  }

  barrier.updateAngleTrig();
  Serial.println(state);
}

