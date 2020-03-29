#include <SoftwareSerial.h>

/*Example sketch to control a stepper motor with A4988/DRV8825 stepper motor driver and Arduino without a library. More info: https://www.makerguides.com */
// Define stepper motor connections and steps per revolution:
#define PINKY_DIRECTION 5
#define RING_DIRECTION 6
#define POINTER_DIRECTION 7

#define PINKY_STEP 2
#define RING_STEP 3
#define POINTER_STEP 4

#define Enable 8

#define STEPS_PER_REVOLUTION 200

SoftwareSerial mySerial(10, 11); // RX, TX
String command = "";             // Stores response of bluetooth device

int INITIAL_STATUS = HIGH;

int MINIMUM_DELAY = 70;
int MAX_REVOLUTIONS = 45;
int POSITIONS = 10;

int REVOLUTIONS_PER_POSITION = MAX_REVOLUTIONS / POSITIONS;

int pinkyPosition = 0;
int ringPosition = 0;
int pointerPosition = 0;

int count = 0;

int chooseStepFinger(int finger)
{
  if (finger == 1)
  {
    return PINKY_STEP;
  }
  else if (finger == 2)
  {
    return RING_STEP;
  }
  else if (finger == 3)
  {
    return POINTER_STEP;
  }
}

int chooseDirectionFinger(int finger)
{
  if (finger == 1)
  {
    return PINKY_DIRECTION;
  }
  else if (finger == 2)
  {
    return RING_DIRECTION;
  }
  else if (finger == 3)
  {
    return POINTER_DIRECTION;
  }
}

// the setup function runs once when you press reset or power the board
void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  // Declare pins as output:
  pinMode(PINKY_STEP, OUTPUT);
  pinMode(RING_STEP, OUTPUT);
  pinMode(POINTER_STEP, OUTPUT);

  pinMode(PINKY_DIRECTION, OUTPUT);
  pinMode(RING_DIRECTION, OUTPUT);
  pinMode(POINTER_DIRECTION, OUTPUT);
  pinMode(Enable, OUTPUT);

  digitalWrite(Enable, INITIAL_STATUS);
  displayMotorStatus();
  if (!disabled())
  {
    for (int i = 0; i < 4; i++)
    {
      runMotor(i, MINIMUM_DELAY, -POSITIONS);
    }
  }
}

// the loop function runs over and over again forever
void loop()
{
  bluetooth();
}

bool disabled()
{
  return digitalRead(Enable);
}

void displayMotorStatus()
{
  if (disabled())
  {
    Serial.println("MOTORS DISABLED");
    digitalWrite(LED_BUILTIN, LOW);
  }
  else
  {
    Serial.println("MOTORS ENABLED");
    digitalWrite(LED_BUILTIN, HIGH);
  }
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

int chooseDirection(int fingerPosition, int desiredPosition)
{
  if (fingerPosition > desiredPosition)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int convertPositionToRevolutions(int fingerPosition, int desiredPosition)
{
  int absPosition = abs(fingerPosition - desiredPosition);
  return absPosition * REVOLUTIONS_PER_POSITION;
}

void motorStep(int stepFinger, int motorDelay)
{
  digitalWrite(stepFinger, HIGH);
  delayMicroseconds(motorDelay);
  digitalWrite(stepFinger, LOW);
  delayMicroseconds(motorDelay);
}

void setNewFingerPosition(int finger, int desiredPosition)
{
  if (finger == 1)
  {
    pinkyPosition = desiredPosition;
  }
  else if (finger == 2)
  {
    ringPosition = desiredPosition;
  }
  else if (finger == 3)
  {
    pointerPosition = desiredPosition;
  }
}

void runMotor(int finger, int motorDelay, int desiredPosition)
{
  printMotorStatus(finger, motorDelay, desiredPosition);

  int stepFinger = chooseStepFinger(finger);
  int directionFinger = chooseDirectionFinger(finger);

  int fingerPosition = getFingerPosition(finger);

  if (fingerPosition == desiredPosition)
  {
    Serial.println("\n\n");
    Serial.println("====================================");
    Serial.println("FINGER POSITION SAME SAME");
    Serial.println("====================================");
    Serial.println("\n\n");
    return;
  }

  int direction = chooseDirection(fingerPosition, desiredPosition);
  int revolutions = convertPositionToRevolutions(fingerPosition, desiredPosition);

  digitalWrite(directionFinger, direction);
  for (int i = 0; i < revolutions * STEPS_PER_REVOLUTION; i++)
  {
    motorStep(stepFinger, motorDelay);
  }

  setNewFingerPosition(finger, desiredPosition);
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

  if (count < 4)
  {
    if (count == 1)
    {
      runMotor(1, 500, 6);
    }
    if (count == 2)
    {
      runMotor(2, 300, 6);
    }
    if (count == 3)
    {
      runMotor(3, motorDelay, 6);
    }
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
