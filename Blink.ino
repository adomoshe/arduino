#include <Servo.h>
#include <SoftwareSerial.h>

// Define stepper motor connections and steps per revolution:
#define PINKY_PIN 8
#define RING_PIN 7
#define MIDDLE_PIN 6
#define INDEX_PIN 5
#define THUMB_PIN 4
#define THUMB_ANGLE_PIN 3

Servo pinky;
Servo ring;
Servo middle;
Servo pointer;
Servo index;
Servo thumb;
Servo thumbAngle;

SoftwareSerial mySerial(10, 11); // RX, TX
String command = "";             // Stores response of bluetooth device

int MINIMUM_DELAY = 70;

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
    return index;
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
  index.attach(INDEX_PIN);
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
  return desiredP.toInt() * 18;
}

void runMotor(
    String pinkyP,
    String ringP,
    String middleP,
    String indexP,
    String thumbP,
    String thumbAngleP,
    String speed)
{
  // Servo finger = chooseFinger(fingerInt);

  // const int currPinkyP = pinky.read();
  // const int currRingP = pinky.read();
  // const int currMiddleP = middle.read();
  // const int currIndexP = index.read();
  // const int currThumbP = thumb.read();
  // const int thumbAngleP = thumbAngle.read();

  const int toPinkyP = convertPosition(pinkyP);
  const int toRingP = convertPosition(ringP);
  const int toMiddleP = convertPosition(middleP);
  const int toIndexP = convertPosition(indexP);
  const int toThumbP = convertPosition(thumbP);
  // const int toThumbAngleP = convertPosition(thumbAngleP, currThumbAngleP);

  // const int movementInAngles = abs(toPosition - currentPosition);
  // Serial.println("------------------");
  // Serial.println("Pinky Position");
  // Serial.print(toPinkyP);
  // Serial.println("------------------");
  const int speedInt = speed.toInt();
  // const int steps = movementInAngles;
  // for (int i = steps; i > 0; i--)
  // {
  pinky.write(toPinkyP);
  ring.write(toRingP);
  middle.write(toMiddleP);
  index.write(toIndexP);
  thumb.write(toThumbP);
  // delay(speedInt * 10);
  // }
}

void bluetooth()
{
  int finger = 2;
  int motorDelay = 100;
  int desiredPosition = 3;

  if (motorDelay < MINIMUM_DELAY)
  {
    motorDelay = MINIMUM_DELAY;
  }

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
    const String indexPosition = command.substring(3, 4);
    const String thumbPosition = command.substring(4, 5);
    const String thumbAnglePosition = command.substring(5, 6);
    const String speed = command.substring(6, 7);

    runMotor(
        pinkyPosition,
        ringPosition,
        middlePosition,
        indexPosition,
        thumbPosition,
        thumbAnglePosition,
        speed);
    // runMotor(1, ringPosition, speed);
    // runMotor(2, middlePosition, speed);
    // runMotor(3, indexPosition, speed);
    // runMotor(4, thumbPosition, speed);
    // runMotor(5, thumbAnglePosition, speed);

    Serial.println(pinkyPosition);
    Serial.println(ringPosition);
    Serial.println(middlePosition);
    Serial.println(indexPosition);
    Serial.println(thumbPosition);
    Serial.println(thumbAnglePosition);
    Serial.println(speed);

    command = ""; // No repeats
  }
}
