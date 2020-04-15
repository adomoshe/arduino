#include <Servo.h>
#include <SoftwareSerial.h>

// Define stepper motor connections and steps per revolution:
#define PINKY_PIN 40
#define RING_PIN 41
#define POINTER_PIN 42

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
    for (int i = 3; i < 4; i++)
    {
      Servo finger = chooseFinger(i);

      for (int pos = 0; pos < 100; pos++)
      {
        motorStep(finger, pos, MINIMUM_DELAY);
      }
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

void runMotor(int fingerInt, int motorDelay, int desiredPosition)
{
  printMotorStatus(fingerInt, motorDelay, desiredPosition);

  int fingerPosition = getFingerPosition(fingerInt);
  Servo finger = chooseFinger(fingerInt);

  if (fingerPosition == desiredPosition)
  {
    Serial.println("\n\n");
    Serial.println("====================================");
    Serial.println("FINGER POSITION SAME SAME");
    Serial.println("====================================");
    Serial.println("\n\n");
    return;
  }

  for (int pos = 0; pos < 180; pos++)
  {
    motorStep(finger, pos, motorDelay);
  }

  setNewFingerPosition(fingerInt, abs(abs(fingerPosition) - abs(desiredPosition)));
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
    runMotor(1, MINIMUM_DELAY * 3, 3);
    runMotor(2, MINIMUM_DELAY * 3, 5);
    runMotor(3, MINIMUM_DELAY, 0);
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
