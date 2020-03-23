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

int PINKY = PINKY_DIRECTION;

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
  checkMotorStatus();
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

void checkMotorStatus()
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

void printMotorStatus(int finger, int motorDelay, int revolutions, int direction)
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
  Serial.print("Revolutions: ");
  Serial.print(revolutions);
  Serial.println("");
  Serial.println("====================================");
  Serial.print("Direction: ");
  Serial.print(direction);
  Serial.println("");
  Serial.println("====================================");
  Serial.println("\n\n");
}

void motorStep(int stepFinger, int motorDelay)
{
  digitalWrite(stepFinger, HIGH);
  delayMicroseconds(motorDelay);
  digitalWrite(stepFinger, LOW);
  delayMicroseconds(motorDelay);
}

void runMotor(int finger, int motorDelay, int revolutions, int direction)
{
  printMotorStatus(finger, motorDelay, revolutions, direction);

  int stepFinger = chooseStepFinger(finger);
  int directionFinger = chooseDirectionFinger(finger);

  digitalWrite(directionFinger, direction);
  for (int i = 0; i < revolutions * STEPS_PER_REVOLUTION; i++)
  {
    motorStep(stepFinger, motorDelay);
  }
}

void bluetooth()
{
  int motorDelay = 100;
  int finger = 2;
  int revolutions = 20;
  int direction = 1;

  if (count < 4)
  {
    if (count == 1)
    {
      runMotor(1, 500, 40, direction);
    }
    if (count == 2)
    {
      runMotor(2, 300, 40, direction);
    }
    if (count == 3)
    {
      runMotor(3, motorDelay, 40, direction);
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
