#include <Servo.h>

class Barrier 
{
  const int barrierDegPerSec = 22;

  Servo servo;
  // servo-related vars
  long desiredAngle = 0;
  long originalAngle = 0;
  long angleRequestedAt = 0;
  long angle = 0;
  
public:
  Barrier(int servoPin, int initialAngle) 
  {
    servo.attach(servoPin);
    servo.write(initialAngle);

    angle = initialAngle;
    setAngle(initialAngle);
  }

  void setAngle(int desiredAngleLocal) {
    originalAngle = angle;
    desiredAngle = desiredAngleLocal;
    angleRequestedAt = millis();
  }

  void updateAngleTrig() {
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print(", Desired: ");
    Serial.println(desiredAngle);
    if (angle != desiredAngle) {
      long elapsed = millis() - angleRequestedAt;
      long sign = originalAngle > desiredAngle ? -1 : +1;
      angle = sign * elapsed * barrierDegPerSec / 1000 + originalAngle;
      long minRange = min(originalAngle, desiredAngle);
      long maxRange = max(originalAngle, desiredAngle);
      angle = max(min(angle, maxRange), minRange);
      servo.write(angle);
    }
  }

  long getAngle() {
    return angle;
  }
};

