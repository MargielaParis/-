#include <Servo.h>

// Arduino pin assignment

#define PIN_POTENTIOMETER 3 // Potentiometer at Pin A3
#define PIN_IR 0
// Add IR Sensor Definition Here !!!
#define PIN_SERVO 10
#define PIN_LED 9

#define alpha 0.4
#define DIST_MIN 100.0
#define DIST_MAX 250.0 

#define _DUTY_MIN 553  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1476 // servo neutral position (90 degree)
#define _DUTY_MAX 2399 // servo full counter-clockwise position (180 degree)

#define LOOP_INTERVAL 30   // Loop Interval (unit: msec)

Servo myservo;
unsigned long last_loop_time;   // unit: msec
float dist, pre_dist;
float dist_ema, pre_ema;

void setup()
{
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED,1);
  Serial.begin(1000000);
}

void loop()
{
  unsigned long time_curr = millis();
  int a_value, duty;

  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  // Remove Next line !!!
  // Read IR Sensor value !!!
  a_value = analogRead(PIN_IR);
  // Convert IR sensor value into distance !!!
  dist = (6762.0 / (a_value - 9)-4.0) * 10.0 - 60.0;

  // we need distance range filter here !!!
  if (dist < DIST_MIN) {
    dist = pre_dist;
    digitalWrite(PIN_LED,1);
  } else if (dist > DIST_MAX) {
    dist = pre_dist;
    digitalWrite(PIN_LED,1);
  } else {
    pre_dist = dist;
    digitalWrite(PIN_LED,0);
  }
  
  // we need EMA filter here !!!
  dist_ema = (alpha * dist) + (1-alpha) * (pre_ema);

  // map distance into duty
  duty = 553 + (dist_ema -100) * 12.30;
  myservo.writeMicroseconds(duty);

  // print IR sensor value, distnace, duty !!!
  Serial.print("MIN:");      Serial.print(DIST_MIN);
  Serial.print(", IR:");     Serial.print(a_value);
  Serial.print(", dist:");   Serial.print(dist);
  Serial.print(", ema:");    Serial.print(dist_ema);
  Serial.print(", servo:");  Serial.print(duty);
  Serial.print(", MAX:");    Serial.print(DIST_MAX);
  Serial.println("");

  pre_ema = dist_ema;
}
