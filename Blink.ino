#include <Servo.h>
#include <SoftwareSerial.h>

// Define stepper motor connections and steps per revolution:
#define PINKY_PIN 8
#define RING_PIN 7
#define MIDDLE_PIN 6
#define POINTER_PIN 5
#define THUMB_PIN 4
#define THUMB_ANGLE_PIN 3

Servo pinky;
Servo ring;
Servo middle;
Servo pointer;
Servo thumb;
Servo thumbAngle;

SoftwareSerial mySerial(10, 11); // RX, TX
String command = "";             // Stores response of bluetooth device

int currentPinkyP = 0;
int currentRingP = 0;
int currentMiddleP = 0;
int currentPointerP = 0;
int currentThumbP = 0;
int currentThumbAngleP = 0;

const int DEGREE_MULTIPLIER = 18;

Servo chooseFinger(int finger)
{
  if (finger == 0)
  {
    return pinky;
  }
  else if (finger == 1)
  {
    return ring;
  }
  else if (finger == 2)
  {
    return middle;
  }
  else if (finger == 3)
  {
    return pointer;
  }
  else if (finger == 4)
  {
    return thumb;
  }
  else if (finger == 5)
  {
    return thumbAngle;
  }
}

int getFingerPosition(int finger)
{
  if (finger == 0)
  {
    return currentPinkyP;
  }
  else if (finger == 1)
  {
    return currentRingP;
  }
  else if (finger == 2)
  {
    return currentMiddleP;
  }
  else if (finger == 3)
  {
    return currentPointerP;
  }
  else if (finger == 4)
  {
    return currentThumbP;
  }
  else if (finger == 5)
  {
    return currentThumbAngleP;
  }
}

void setFingerPosition(int finger, int newPosition)
{
  if (finger == 0)
  {
    currentPinkyP = newPosition;
  }
  else if (finger == 1)
  {
    currentRingP = newPosition;
  }
  else if (finger == 2)
  {
    currentMiddleP = newPosition;
  }
  else if (finger == 3)
  {
    currentPointerP = newPosition;
  }
  else if (finger == 4)
  {
    currentThumbP = newPosition;
  }
  else if (finger == 5)
  {
    currentThumbAngleP = newPosition;
  }
}

void initialize()
{
  for (int i = 0; i < 6; i++)
  {
    Servo finger = chooseFinger(i);
    motorStep(finger, 0);
  }
}

// the setup function runs once when you press reset or power the board
void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  pinky.attach(PINKY_PIN);
  ring.attach(RING_PIN);
  middle.attach(MIDDLE_PIN);
  pointer.attach(POINTER_PIN);
  thumb.attach(THUMB_PIN);
  thumbAngle.attach(THUMB_ANGLE_PIN);

  displayMotorStatus();
  initialize();
}

// the loop function runs over and over again forever
void loop()
{
  bluetooth();
}

void displayMotorStatus()
{
  Serial.println("MOTORS ENABLED");
  digitalWrite(LED_BUILTIN, HIGH);
}

void motorStep(Servo finger, int pos)
{
  finger.write(pos);
}

int convertPosition(String desiredP)
{
  // const int mapValue = map(desiredP.toInt(), 0, 1, 0, 180);
  return desiredP.toInt() * DEGREE_MULTIPLIER;
}

void runMotor(
    String pinkyP,
    String ringP,
    String middleP,
    String pointerP,
    String thumbP,
    String thumbAngleP,
    String speed)
{

  const int toPinkyP = convertPosition(pinkyP);
  const int toRingP = convertPosition(ringP);
  const int toMiddleP = convertPosition(middleP);
  const int toPointerP = convertPosition(pointerP);
  const int toThumbP = convertPosition(thumbP);

  const int toPositions[] = {toPinkyP, toRingP, toMiddleP, toPointerP, toThumbP};
  // const int toThumbAngleP = convertPosition(thumbAngleP, currThumbAngleP);

  const int speedInt = speed.toInt();

  Serial.println(currentPinkyP);
  Serial.println(currentRingP);
  Serial.println(currentMiddleP);
  Serial.println(currentPointerP);
  Serial.println(currentThumbP);

  for (int pos = 0; pos < 180; pos++)
  {
    for (int finger = 0; finger < 6; finger++)
    {
      Servo servoFinger = chooseFinger(finger);
      const int fingerPosition = getFingerPosition(finger);
      const int toPosition = toPositions[finger];
      const int toPositionDeg = toPosition * DEGREE_MULTIPLIER;

      if (fingerPosition > toPositionDeg)
      {
        const int newPosition = fingerPosition - 1;
        servoFinger.write(newPosition);
        setFingerPosition(finger, newPosition);
        delay(1);
      }
      else if (fingerPosition < toPositionDeg)
      {
        const int newPosition = fingerPosition + 1;
        servoFinger.write(newPosition);
        setFingerPosition(finger, newPosition);
        delay(1);
      }
    }
    delay(speedInt * 2);
  }
}

void bluetooth()
{
  if (Serial.available())
  {
    while (Serial.available())
    { // While there is more to be read, keep reading.
      command += (char)Serial.read();
    }
    Serial.println(command);
    const String pinkyPosition = command.substring(0, 1);
    const String ringPosition = command.substring(1, 2);
    const String middlePosition = command.substring(2, 3);
    const String pointerPosition = command.substring(3, 4);
    const String thumbPosition = command.substring(4, 5);
    const String thumbAnglePosition = command.substring(5, 6);
    const String speed = command.substring(6, 7);

    runMotor(
        pinkyPosition,
        ringPosition,
        middlePosition,
        pointerPosition,
        thumbPosition,
        thumbAnglePosition,
        speed);

    Serial.println(pinkyPosition);
    Serial.println(ringPosition);
    Serial.println(middlePosition);
    Serial.println(pointerPosition);
    Serial.println(thumbPosition);
    Serial.println(thumbAnglePosition);
    Serial.println(speed);

    command = ""; // No repeats
  }
}
