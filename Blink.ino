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

int pinkyPosition = 10;
int ringPosition = 10;
int pointerPosition = 10;

int count = 0;

Servo chooseFinger(int finger)
{
  if (finger == 1)
  {
    return pinky;
  }
  else if (finger == 2)
  {
    return ring;
  }
  else if (finger == 3)
  {
    return pointer;
  }
}

void initialize()
{
  for (int i = 1; i < 3; i++)
  {
    Servo finger = chooseFinger(i);
    motorStep(finger, 0, 0);
  }
}

// the setup function runs once when you press reset or power the board
void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  // Declare pins as output:
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

void printMotorStatus(int finger, int motorDelay, int position)
{
  Serial.println("====================================");
  Serial.print("Finger: ");
  Serial.print(finger);
  Serial.println("");
  Serial.println("====================================");
  Serial.print("Motor Delay: ");
  Serial.print(motorDelay);
  Serial.println("");
  Serial.println("====================================");
  Serial.print("Position: ");
  Serial.print(position);
  Serial.println("");
  Serial.println("====================================");

  Serial.println("\n\n");
}

int getFingerPosition(int finger)
{
  if (finger == 1)
  {
    return pinkyPosition;
  }
  else if (finger == 2)
  {
    return ringPosition;
  }
  else if (finger == 3)
  {
    return pointerPosition;
  }
}

void motorStep(Servo finger, int pos, int motorDelay)
{
  finger.write(pos);
  delay(motorDelay);
}

void runMotor(int fingerInt, int motorDelay, int desiredPosition)
{
  printMotorStatus(fingerInt, motorDelay, desiredPosition);

  Servo finger = chooseFinger(fingerInt);
  int currentPosition = finger.read();

  if (desiredPosition > 180)
  {
    Serial.println("FINGER POSITION TOO BIG");
    return;
  }
  if (currentPosition == desiredPosition)
  {
    Serial.println("\n\n");
    Serial.println("====================================");
    Serial.println("FINGER POSITION SAME SAME");
    Serial.println("====================================");
    Serial.println("\n\n");
    return;
  }

  if (currentPosition > desiredPosition)
  {
    for (int pos = currentPosition + 1; pos < desiredPosition; pos++)
    {
      motorStep(finger, pos, motorDelay);
    }
  }
  else
  {
    for (int pos = currentPosition - 1; pos > desiredPosition; pos--)
    {
      motorStep(finger, pos, motorDelay);
    }
  }
}

int convertPosition(String position)
{
  return position.toInt() * 18;
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

  if (count < 1)
  {
    motorStep(pinky, 100, 10);
    motorStep(ring, 100, 10);
    // runMotor(3, 20, 10);
    count++;
  }
  delay(300);
  if (count == 1)
  {
    motorStep(pinky, 50, 120);
    motorStep(ring, 50, 60);
    count++;
  }
  delay(300);

  if (count == 2)
  {
    motorStep(pinky, 0, 50);
    motorStep(ring, 0, 50);
    count++;
  }
  delay(300);

  if (count == 3)
  {
    motorStep(pinky, 100, 50);
    motorStep(ring, 100, 50);
    count++;
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

    motorStep(pinky, convertPosition(pinkyPosition), speed.toInt());
    motorStep(ring, convertPosition(pinkyPosition), speed.toInt());
    motorStep(middle, convertPosition(pinkyPosition), speed.toInt());
    motorStep(pointer, convertPosition(pinkyPosition), speed.toInt());
    motorStep(index, convertPosition(pinkyPosition), speed.toInt());
    motorStep(thumb, convertPosition(pinkyPosition), speed.toInt());
    motorStep(thumbAngle, convertPosition(pinkyPosition), speed.toInt());

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
