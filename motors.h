// this #ifndef stops this file
// from being included mored than
// once by the compiler. 
# ifndef _MOTORS_H
# define _MOTORS_H
# define L_PWM_PIN 10
# define L_DIR_PIN 16
# define R_PWM_PIN 9
# define R_DIR_PIN 15

# define FWD LOW
# define REV HIGH


// Class to operate the motor(s).
class Motors_c {
  public:

    // Constructor, must exist.
    Motors_c() {

    } 

    // Use this function to 
    // initialise the pins and 
    // state of your motor(s).
    void initialise() {
      pinMode(L_PWM_PIN, OUTPUT);
      pinMode(L_DIR_PIN, OUTPUT);
      pinMode(R_PWM_PIN, OUTPUT);
      pinMode(R_DIR_PIN, OUTPUT);

  // Set initial direction (HIGH/LOW)
  // for the direction pins.
      digitalWrite(L_DIR_PIN, FWD);
      digitalWrite(R_DIR_PIN, FWD);
  
  // Set initial power values for the PWM
  // Pins
  analogWrite(L_PWM_PIN, 0);
  analogWrite(R_PWM_PIN, 0);
  delay(500);
    }

    void leftMotorDir(float left_pwm, bool dir){
        digitalWrite(L_DIR_PIN, dir);
        analogWrite(L_PWM_PIN, left_pwm);
        }
        
    void rightMotorDir(float right_pwm, bool dir){
      digitalWrite(R_DIR_PIN, dir);
      analogWrite(R_PWM_PIN, right_pwm);
      }
      

    void setMotorPower( float left_pwm, float right_pwm ){
      // your motor(s)
    // ...
    if (left_pwm < 0 && right_pwm < 0){
      left_pwm = abs(left_pwm);
      right_pwm = abs(right_pwm);
      leftMotorDir(left_pwm, REV);
      rightMotorDir(right_pwm, REV);
      }

      
     else if(left_pwm < 0 && right_pwm > 0) {
      left_pwm = abs(left_pwm);
      leftMotorDir(left_pwm, REV);
      rightMotorDir(right_pwm, FWD);
      }

      else if (left_pwm > 0 && right_pwm < 0){
      right_pwm = abs(right_pwm);
      leftMotorDir(left_pwm, FWD);
      rightMotorDir(right_pwm, REV);
      }

      else {
      analogWrite(L_PWM_PIN, left_pwm);
      analogWrite(R_PWM_PIN, right_pwm);
        }
      }
};



#endif
