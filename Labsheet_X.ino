#include "motors.h"
#include "linesensor.h"
#include "encoders.h"
#include "kinematics.h"
#include "pid.h"

# define threashold_ts 720
# define LINE_SENSOR_UPDATE 3200
# define MOTOR_UPDATE       2000

unsigned long motor_ts;    
unsigned long line_sensor_ts;
const int MaxSensors = 3;  // Define the number of line sensors
int SensorPins[MaxSensors] = {18, 20, 21};  // Define sensor pins
unsigned long Rn[MaxSensors];  // Array to store sensor readings
float Dw[2];        // Array to store normalise reading
const int BiasPWM = 20; 
const int MaxTurnPWM = 25;
unsigned long current_ts;
unsigned long elapsed_t;

LineSensor_c line_sensors; // Line sensor variables/functions
Motors_c motors;           // Motor variables/functions

void setup() {

  // initialise the sensors
  line_sensors.initialise();
  motors.initialise();

  // Set initial timestamp values
  motor_ts       = micros();
  line_sensor_ts = micros();
  Serial.begin(9600);
  //delay(1500);
  Serial.println("***RESET***");
}

void loop() {
  current_ts = micros();
  elapsed_t = current_ts - line_sensor_ts;
  if (elapsed_t>LINE_SENSOR_UPDATE){
    setBotDir();
    line_sensor_ts = micros();
  }
  //elapsed_t = current_ts - line_sensor_ts;
  Serial.println((String)"Elapsed_t "+elapsed_t);
}

void setBotDir(){
  if (feedback(1,2,3)){
    float w = weightedMeasurement();
    Serial.println(w);
    float LeftPWM = BiasPWM + (w * MaxTurnPWM);
    float RightPWM = BiasPWM - (w * MaxTurnPWM);
    motors.setMotorPower(LeftPWM, RightPWM);
  //}
  }
  else {
    motors.setMotorPower(0.0, 0.0);
    }
  }

float weightedMeasurement(){
   
    for (int n=0; n<MaxSensors; n++){
      int m = n+1;
      Rn[n] = line_sensors.readLineSensor(m);
      }
      //Serial.println((String)"Dn2 "+Rn[0]);
      //Serial.println((String)"Dn4 "+Rn[2]);
      // Sum of Dn2 & Dn4
      float sum = Rn[0] + Rn[2];
      //Serial.println((String)"Sum "+sum);
      // Normalise the reading
      int x[2] = {0,2};
      for (int n=0; n<2; n++){
        float Nn = Rn[x[n]]/sum;
        Dw[n] = Nn * 2.0;
        }
        float W = Dw[1] - Dw[0];
        return W;
    }

bool feedback(int ls, int ms, int rs){
  // Check the line for the first time
  if (line_sensors.readLineSensor(ls) < threashold_ts && line_sensors.readLineSensor(ms) < threashold_ts && line_sensors.readLineSensor(rs) < threashold_ts){
    return false;
  }
  else {
    return true;
    }
}
