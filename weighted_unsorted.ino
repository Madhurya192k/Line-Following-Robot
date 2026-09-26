#include "encoders.h"
//#include "linesensor.h"

// Motor pins
#define RIGHT_MOTOR_SPEED 9
#define RIGHT_MOTOR_DIRECTION 15
#define LEFT_MOTOR_SPEED 10
#define LEFT_MOTOR_DIRECTION 16

// Sensor pins
#define EMIT_PIN 11
#define LS_EXTREME_LEFT_PIN 12 // DN1 pin
#define LS_LEFT_PIN 18
#define LS_MIDDLE_PIN 20
#define LS_RIGHT_PIN 21
#define LS_EXTREME_RIGHT_PIN 22 // DN5 pin
#define NUM_SENSORS 5

int SENSOR_PINS[NUM_SENSORS] = {LS_EXTREME_LEFT_PIN, LS_LEFT_PIN, LS_MIDDLE_PIN, LS_RIGHT_PIN, LS_EXTREME_RIGHT_PIN};
float sensor_values[NUM_SENSORS];


  unsigned long readSensor(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  delayMicroseconds(10);
  pinMode(pin, INPUT);
  unsigned long start_time = micros();
  while (digitalRead(pin) == HIGH) {}
  return micros() - start_time;
}

#define BLACK_LINE_THRESHOLD 1600

#define BIAS_PWM 25
#define MAX_TURN_PWM 20

void setMotorPower(int leftMotorSpeed, int rightMotorSpeed) {
  analogWrite(LEFT_MOTOR_SPEED, abs(leftMotorSpeed) );
  digitalWrite(LEFT_MOTOR_DIRECTION, leftMotorSpeed < 0 ? HIGH : LOW); // Add this line to control direction
  analogWrite(RIGHT_MOTOR_SPEED, abs(rightMotorSpeed) );
  digitalWrite(RIGHT_MOTOR_DIRECTION, rightMotorSpeed < 0 ? HIGH : LOW); // Add this line to control direction
}

bool lineDetected = false;

//inesensor readvalues();
void drive_forwards(){
  while(count_e1<670 and count_e0<670)
   {
       setMotorPower(25,25);      
       delay(1000);
   } //if(readSensor()>threshold)
   setMotorPower(0,0);
   delay(100);
}



void setup() {
  // Set some initial pin modes and states
  pinMode(EMIT_PIN, OUTPUT);
  digitalWrite(EMIT_PIN, HIGH); // Set EMIT as an input (off)

  // Set line sensor pins to input
  pinMode(LS_EXTREME_LEFT_PIN, INPUT);
  pinMode(LS_LEFT_PIN, INPUT);
  pinMode(LS_MIDDLE_PIN, INPUT);
  pinMode(LS_RIGHT_PIN, INPUT);
  pinMode(LS_EXTREME_RIGHT_PIN, INPUT);

  // Set motor pins to output
  pinMode(RIGHT_MOTOR_SPEED, OUTPUT);
  pinMode(RIGHT_MOTOR_DIRECTION, OUTPUT);
  pinMode(LEFT_MOTOR_SPEED, OUTPUT);
  pinMode(LEFT_MOTOR_DIRECTION, OUTPUT);

  // Start Serial, wait to connect, print a debug message.
  Serial.begin(9600);
  delay(1500);
  Serial.println("RESET");
  
  //setup encoder 
  setupEncoder0();
  setupEncoder1();
}
void loop() {
  if(!lineDetected){
  drive_forwards();
  lineDetected=true;
  }
 while(count_e0<8750 && count_e0<8750){
  // Turn on the emitter pin
  pinMode(EMIT_PIN, OUTPUT);
  digitalWrite(EMIT_PIN, HIGH);

   //Read from sensors
    for (int i = 0; i < NUM_SENSORS; i++) {
    sensor_values[i] = (float)readSensor(SENSOR_PINS[i]);
    }
  //Turn off the emitter pin
  digitalWrite(EMIT_PIN, LOW);
  delay(10);

  
 int sum_weighted_values = (sensor_values[1])* -1.0 + (sensor_values[2] * 0.0) + sensor_values[3] * 1.0;
float sum = abs(sensor_values[1]) + abs(sensor_values[3]);

float n1,n3;
n1 = (sensor_values[1] / sum)*2.0;
n3 = (sensor_values[3] / sum)*2.0;

float position = n1 - n3;

    int left_pwm = BIAS_PWM - (position * MAX_TURN_PWM);
    int right_pwm = BIAS_PWM + (position * MAX_TURN_PWM);

 unsigned start_time=micros();
  
if (sensor_values[1] > BLACK_LINE_THRESHOLD) { // If DN2 is above threshold, turn left
    setMotorPower(-MAX_TURN_PWM, MAX_TURN_PWM);
} else if (sensor_values[2] > BLACK_LINE_THRESHOLD) { // If DN3 is above threshold, go straight
    setMotorPower(left_pwm , right_pwm);
} else if (sensor_values[3] > BLACK_LINE_THRESHOLD) { // If DN4 is above threshold, turn right
    setMotorPower(MAX_TURN_PWM, -MAX_TURN_PWM);
} else if (sensor_values[0] > BLACK_LINE_THRESHOLD) { // If DN1 is above threshold, emergency turn left
    setMotorPower(-MAX_TURN_PWM, MAX_TURN_PWM);
} else if (sensor_values[4] > BLACK_LINE_THRESHOLD) { // If DN5 is above threshold, emergency turn right
    setMotorPower(MAX_TURN_PWM, -MAX_TURN_PWM);   
}
else { // If no sensor detects line, it's a 180 degrees turn
    setMotorPower(MAX_TURN_PWM, -MAX_TURN_PWM);
    //delay(200); // Adjust delay as needed for a full turn
}
unsigned end_time=micros();
    if ((end_time-start_time)>100){
      //  Serial.println(state_e0);
      //  Serial.println(state_e1);
        Serial.println(count_e0);
       // Serial.println(count_e0);
  Serial.println(count_e1);
  delay(100);
    }
   
 }
 setMotorPower(0, 0);
 }
