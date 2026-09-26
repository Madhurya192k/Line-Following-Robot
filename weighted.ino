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

#define BLACK_LINE_THRESHOLD 1400

#define NUM_SENSORS 5
int SENSOR_PINS[NUM_SENSORS] = {LS_EXTREME_LEFT_PIN, LS_LEFT_PIN, LS_MIDDLE_PIN, LS_RIGHT_PIN, LS_EXTREME_RIGHT_PIN};
#define BIAS_PWM 25
#define MAX_TURN_PWM 25

unsigned long readSensor(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  delayMicroseconds(10);
  pinMode(pin, INPUT);
  unsigned long start_time = micros();
  while (digitalRead(pin) == HIGH) {}
  return micros() - start_time;
}

void setMotorPower(int leftMotorSpeed, int rightMotorSpeed) {
  analogWrite(LEFT_MOTOR_SPEED, leftMotorSpeed);
  digitalWrite(LEFT_MOTOR_DIRECTION, leftMotorSpeed > 0 ? HIGH : LOW); // Add this line to control direction
  analogWrite(RIGHT_MOTOR_SPEED, rightMotorSpeed);
  digitalWrite(RIGHT_MOTOR_DIRECTION, rightMotorSpeed > 0 ? HIGH : LOW); // Add this line to control direction
}

void setup() {
  // Set some initial pin modes and states
  pinMode(EMIT_PIN, INPUT); // Set EMIT as an input (off)
  
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
  Serial.println("*RESET*");
}

void loop() {
  int sensor_values[NUM_SENSORS] = {0};
  
  // Turn on the emitter pin
  pinMode(EMIT_PIN, OUTPUT);

  // Read from sensors
  for (int i = 0; i < NUM_SENSORS; i++) {
    sensor_values[i] = readSensor(SENSOR_PINS[i]);
  }

  // Turn off the emitter pin
  pinMode(EMIT_PIN, LOW);
  delay(25);

  // Calculate weighted measurement for DN2, DN3, DN4
  int sum_weighted_values = sensor_values[1] * -1 + sensor_values[2] * +0 + sensor_values[3] * +1;
  int sum_weights = sensor_values[1] + sensor_values[2] + sensor_values[3];

  int position = sum_weights !=0 ? sum_weighted_values / sum_weights :0;

  // Now you can use the position value to control your motors.
  if (sensor_values[0] > BLACK_LINE_THRESHOLD) { // DN1 is detected.
    setMotorPower(30 , -30); // Turn left.

  } else if (sensor_values[4] > BLACK_LINE_THRESHOLD && sensor_values[0] <= BLACK_LINE_THRESHOLD) { // DN5 is detected and DN1 is not detected.
    setMotorPower(-30 , 30); // Turn right.

  } else if (sum_weights > BLACK_LINE_THRESHOLD) { // Line is detected by DN2 , DN3 , or DN4.
    int pwm = BIAS_PWM + (position * MAX_TURN_PWM);
    setMotorPower(pwm , pwm);

  } else { // Line is not detected.
    setMotorPower(-30 , -30); // Make a U-turn.

  }
}
