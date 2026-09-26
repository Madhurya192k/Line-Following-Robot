#include "line_sensors.h"
#include "motors.h"

#define MAX_SPEED 80
#define ACTIVE_THRESHOLD 1200

#define BUZZER_PIN 6

#define STATE_INITIAL 0
#define STATE_FOUND_LINE 1
#define STATE_NOT_ON_LINE 2
#define STATE_END_OF_LINE 3

#define YELLOW_LED 13
#define GREEN_LED 30
#define RED_LED 17

Motors_c motors;
LineSensors_c ls;

int state = STATE_INITIAL;

// for the joinLine()
int sensorStateChange = 0;
int sensorStateChanges = 3;
bool joinLine_online = false;
bool joinLine_currentState = joinLine_online;
bool joinLine_previousState = joinLine_currentState;

// for the performUTurn()
unsigned int offlineCounter = 0;
unsigned int offlineCounter_max = 60;

// notes down the start time - to help identify end of line
unsigned int runtime_start = millis();

void setup()
{
  motors.init(MAX_SPEED);
  ls.init(ACTIVE_THRESHOLD);

  state = STATE_INITIAL;

  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  Serial.begin(9600);
  delay(1000);
  Serial.println("***RESET***");
}

void loop()
{
  ls.updateSensors();

  // State changes occur inside their respective functions
  if (state == STATE_INITIAL)
  {
    joinLine();
  }
  else if (state == STATE_FOUND_LINE)
  {
    followLine();
  }
  else if (state == STATE_END_OF_LINE)
  {
    stopRobot();
  }

  delay(25);
}

void followLine()
{
  if (ls.right_online)
  {
    offlineCounter = 0;

    turnRight45();
  }
  else if (ls.right_online && ls.left_online)
  {
    offlineCounter = 0;

    turnRight45();
  }
  else if (not ls.right_online && ls.left_online)
  {
    offlineCounter = 0;

    turnLeft45();
  }
  else if (not ls.midleft_online && ls.middle_online && ls.midright_online)
  {
    offlineCounter = 0;

    motors.setMotorPower(20, 20);
  }
  else if (ls.midleft_online && ls.middle_online && not ls.midright_online)
  {
    offlineCounter = 0;

    motors.setMotorPower(20, 30);
  }
  else if (not ls.middle_online && ls.midright_online)
  {
    offlineCounter = 0;

    motors.setMotorPower(30, 20);
  }

  else if (not ls.midleft_online && not ls.middle_online && not ls.midright_online)
  {

    offlineCounter++;

    motors.setMotorPower(17, 17);

    if (((millis() - runtime_start) > 50000) && (offlineCounter >= offlineCounter_max))
    {
      state = STATE_END_OF_LINE;
    }

    else if (offlineCounter == offlineCounter_max)
    {
      performUTurn();
    }
  }
  else
  {
    motors.setMotorPower(20, 20);
  }
}

// stops the robot indefinitely and plays the buzzer
void stopRobot()
{
  motors.setMotorPower(0, 0);
  while (true)
  {
    buzzerBeep();
    delay(500);
  }
}

// plays a beep on the buzzer
void buzzerBeep()
{
  analogWrite(BUZZER_PIN, 10);
  delay(200);
  analogWrite(BUZZER_PIN, 0);
}

// leaves the box to join the line
// Moves through 3 state changes of LOW, HIGH, LOW, HIGH to join the line
void joinLine()
{
  motors.setMotorPower(20, 20);
  if ((ls.midleft_sensor >= ACTIVE_THRESHOLD) &&
      (ls.middle_sensor >= ACTIVE_THRESHOLD) &&
      (ls.midright_sensor >= ACTIVE_THRESHOLD))
  {
    joinLine_online = true;
    joinLine_currentState = true;
  }
  else
  {
    joinLine_online = false;
    joinLine_currentState = false;
  }

  if (joinLine_previousState != joinLine_currentState)
  {
    sensorStateChange++;
  }
  joinLine_previousState = joinLine_currentState;

  if (sensorStateChange == sensorStateChanges)
  {
    state = STATE_FOUND_LINE;
    sensorStateChange++;
  }
}

void performUTurn()
{
  motors.setMotorPower(-20, 20);
  delay(1800);
  motors.setMotorPower(20, 20);
  delay(1000);
}

void turnLeft45()
{
  motors.setMotorPower(-20, 20);
  delay(500);
  motors.setMotorPower(20, 20);
  delay(200);
  motors.setMotorPower(0, 0);
}

void turnRight45()
{
  motors.setMotorPower(20, -20);
  delay(500);
  motors.setMotorPower(20, 20);
  delay(200);
  motors.setMotorPower(0, 0);
}

void redLED()
{
  // RED LED
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
}
