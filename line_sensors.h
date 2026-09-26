#define EMIT_PIN 11

#define LS_LEFT_PIN 12
#define LS_MIDLEFT_PIN 18
#define LS_MIDDLE_PIN 20
#define LS_MIDRIGHT_PIN 21
#define LS_RIGHT_PIN 22

int ls_pins[5] = {LS_LEFT_PIN, LS_MIDLEFT_PIN, LS_MIDDLE_PIN, LS_MIDRIGHT_PIN,
                  LS_RIGHT_PIN};

class LineSensors_c
{

public:
  int active_threshold = 0;

  int left_sensor = 0;
  int midleft_sensor = 0;
  int middle_sensor = 0;
  int midright_sensor = 0;
  int right_sensor = 0;

  // to make sensors easier to work with
  bool left_online = false;
  bool midleft_online = false;
  bool middle_online = false;
  bool midright_online = false;
  bool right_online = false;

  LineSensors_c() {}

  // Line sensors setup
  void init(int act_threshold)
  {
    pinMode(EMIT_PIN, INPUT);

    pinMode(LS_LEFT_PIN, INPUT);
    pinMode(LS_MIDLEFT_PIN, INPUT);
    pinMode(LS_MIDDLE_PIN, INPUT);
    pinMode(LS_MIDRIGHT_PIN, INPUT);
    pinMode(LS_RIGHT_PIN, INPUT);

    active_threshold = act_threshold;
  }

  void updateSensors()
  {
    left_sensor = readLineSensor(0);
    midleft_sensor = readLineSensor(1);
    middle_sensor = readLineSensor(2);
    midright_sensor = readLineSensor(3);
    right_sensor = readLineSensor(4);

    updateLineStatus();
  }

  void updateLineStatus()
  {

    // left_sensor
    if (left_sensor < active_threshold)
    {
      left_online = false;
    }
    else
    {
      left_online = true;
    }

    // midleft_sensor
    if (midleft_sensor < active_threshold)
    {
      midleft_online = false;
    }
    else
    {
      midleft_online = true;
    }

    // middle_sensor
    if (middle_sensor < active_threshold)
    {
      middle_online = false;
    }
    else
    {
      middle_online = true;
    }

    // midright_sensor
    if (midright_sensor < active_threshold)
    {
      midright_online = false;
    }
    else
    {
      midright_online = true;
    }

    // right_sensor
    if (right_sensor < active_threshold)
    {
      right_online = false;
    }
    else
    {
      right_online = true;
    }
  }

  float readLineSensor(int sensorNum)
  {
    if (sensorNum < 0)
    {
      return -1;
    }
    if (sensorNum > 4)
    {
      return -1;
    }

    pinMode(EMIT_PIN, OUTPUT);
    digitalWrite(EMIT_PIN, HIGH);

    pinMode(ls_pins[sensorNum], OUTPUT);
    digitalWrite(ls_pins[sensorNum], HIGH);

    delayMicroseconds(10);

    pinMode(ls_pins[sensorNum], INPUT);

    unsigned long start_time = micros();

    while (digitalRead(ls_pins[sensorNum]) == HIGH)
    {
      // Remove repeated initialisation
      unsigned long current_time = micros();

      if ((current_time - start_time) > 5000)
      {
        Serial.println("Time-out. Breaking out of while-loop!");
        break;
      }
    }

    unsigned long end_time = micros();

    pinMode(EMIT_PIN, INPUT);

    unsigned long elapsed_time = end_time - start_time;

    return elapsed_time;
  }

  // for debugging
  void printSensors()
  {
    Serial.print(left_sensor);
    Serial.print(", ");
    Serial.print(midleft_sensor);
    Serial.print(", ");
    Serial.print(middle_sensor);
    Serial.print(", ");
    Serial.print(midright_sensor);
    Serial.print(", ");
    Serial.println(right_sensor);
  }

  // for debugging
  void printActivations()
  {
    Serial.print(left_online);
    Serial.print(", ");
    Serial.print(midleft_online);
    Serial.print(", ");
    Serial.print(middle_online);
    Serial.print(", ");
    Serial.print(midright_online);
    Serial.print(", ");
    Serial.println(right_online);
  }
};
