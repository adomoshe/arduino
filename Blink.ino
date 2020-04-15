#include <Servo.h>
#include <SoftwareSerial.h>

// Define stepper motor connections and steps per revolution:
#define PINKY_PIN 9
#define RING_PIN 10
#define POINTER_PIN 5

Servo pinky;
Servo ring;
Servo pointer;

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
  pointer.attach(POINTER_PIN);

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

  if (mySerial.available())
  {
    while (mySerial.available())
    { // While there is more to be read, keep reading.
      command += (char)mySerial.read();
    }
    Serial.println(command);
    command = ""; // No repeats
  }

  // Read user input if available.
  if (Serial.available())
  {
    delay(10); // The DELAY!
    mySerial.write(Serial.read());
  }
}
