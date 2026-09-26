// this #ifndef stops this file
// from being included mored than
// once by the compiler. 
#ifndef _LINESENSOR_H
#define _LINESENSOR_H
# define EMIT_PIN    11    // Documentation says 11.
# define LS_LEFT_PIN 12   // Complete for DN1 pin
# define LS_MIDLEFT_PIN 18   // Complete for DN2 pin
# define LS_MIDDLE_PIN 20   // Complete for DN3 pin
# define LS_MIDRIGHT_PIN 21   // Complete for DN4 pin
# define LS_RIGHT_PIN 22   // Complete for DN5 pin

// Class to operate the linesensor(s).
class LineSensor_c {
  public:
  
    // Constructor, must exist.
    LineSensor_c() {

    }

// Store our pin numbers into an array, which means
// we can conveniently select one later.
// ls(line sensor)_pin
int ls_pins[5] = {LS_LEFT_PIN,
                  LS_MIDLEFT_PIN,
                  LS_MIDDLE_PIN,
                  LS_MIDRIGHT_PIN,
                  LS_RIGHT_PIN };

void initialise() {

  // Set some initial pin modes and states
  pinMode( EMIT_PIN, INPUT ); // Set EMIT as an input (off)
  pinMode( LS_LEFT_PIN, INPUT );     // Set line sensor pin to input
  pinMode( LS_MIDLEFT_PIN, INPUT );
  pinMode( LS_MIDLEFT_PIN, INPUT );
  pinMode( LS_MIDLEFT_PIN, INPUT );
  pinMode( LS_MIDLEFT_PIN, INPUT );

} // End of setup()

float readLineSensor( int number ) {

    // These two if statements should be
    // completed to prevent a memory error.
    // What would be a good value to return that
    // would not be mistaken for a sensor reading?
    if( number < 0 ) {
        return 0;
    }
    if( number > 4 ) {
        return 0;
    }
    pinMode( EMIT_PIN, OUTPUT );
    digitalWrite( EMIT_PIN, HIGH );

    // In this line, we retrieve the pin value
    // stored in the array "ls_pins" at location
    // "number".  So it is like a look-up table.
    // We can think of ls_pins in memory like:
    // Index 0, Index 1, Index 2, Index 3, Index 4
    //[  DN1  ][   DN2 ][  DN3  ][  DN4  ][  DN5  ]
    pinMode( ls_pins[number], OUTPUT );
    digitalWrite( ls_pins[number], HIGH );
    delayMicroseconds(10);

    // Start discharge
    pinMode(ls_pins[number], INPUT);
    unsigned long start_time = micros();

    while( digitalRead(ls_pins[number]) == HIGH ) {
        // Do nothing here (waiting).
    }

    unsigned long end_time = micros();

    pinMode( EMIT_PIN, INPUT );

    unsigned long elapsed_time = end_time - start_time;

    // Give the result back to wherever this
    // function was called from.
    //Serial.println((String)"DN2 " + elapsed_time);
    return (float)elapsed_time;
}
};



#endif
