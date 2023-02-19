#define PULSE_TIMEOUT_US (4 * 1000)
#define TIMER_INTERVAL_UNKNOWN_UNITS 5
#define SPEED_OF_SOUND_CM_PER_US 0.034
#define N 5

class TrainDetector {
  boolean lastDetectValue = false;
  float buffer[N];
  int bufferIndex = 0;
  int trigPin;
  int echoPin;

public:
  TrainDetector(int trigPin_, int echoPin_) {
    trigPin = trigPin_;
    echoPin = echoPin_;
    
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    
    for (int i = 0; i < N; i++) {
      buffer[i] = 0;
    }
  }

  boolean detectTrain() {
    int count = 0;
    float cumul = 0;
    for (int i = 0; i < 10; i++) {
      // Clears the trigPin
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      // Sets the trigPin on HIGH state for 10 micro seconds
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      //delayMicroseconds(2);
      // Reads the echoPin, returns the sound wave travel time in microseconds
      long duration = pulseIn(echoPin, HIGH, PULSE_TIMEOUT_US);
      // Calculating the distance
      long distance = duration * SPEED_OF_SOUND_CM_PER_US / 2;
      if (distance > 5 && distance < 60) {
        count++;
        cumul += distance;
      }
    }
  
    float averaged = count > 0 ? cumul / count : 0;
    averaged = averaged > 30 ? 0 : averaged;
    buffer[bufferIndex] = averaged;
    bufferIndex++;
    bufferIndex = bufferIndex % N;
  
    bool allZeros = true;
    bool allNonZeros = true;
    for (int i = 0; i < N; i++) {
      allZeros &= buffer[i] == 0;
      allNonZeros &= buffer[i] != 0;
    }
    if (allZeros) {
      lastDetectValue = false;
    }
    if (allNonZeros) {
      lastDetectValue = true;
    }
    // Prints the distance on the Serial Monitor
    Serial.print("Distance, cm: ");
    Serial.println(averaged);
  
    return lastDetectValue;
  }
};

