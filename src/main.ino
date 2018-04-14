#include <LowPower.h>

// misc defines
#define DOWN true
#define UP false

// Configurables
// Motor speed
#define SPEED 128
// Time to sleep after moving door (seconds)
#define SLEEPTIME 600
// Threshold before it is considered 'morning'
#define LIGHT_THRESHOLD 100
// Threshold before it is considered 'night'
#define DARK_THRESHOLD 100

// Pins
#define ROOF_PIN 3
#define FLOOR_PIN 2
#define ENABLE_PIN 11
#define MOTOR_PIN_1 10
#define MOTOR_PIN_2 9
#define PHOTORESISTOR_PIN A0

int sensorValue = 0;
boolean doorStatus;

void setup()
{
  pinMode(MOTOR_PIN_1, OUTPUT);
  pinMode(MOTOR_PIN_2, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(ROOF_PIN, INPUT_PULLUP);
  pinMode(FLOOR_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(ROOF_PIN, HIGH);
  digitalWrite(FLOOR_PIN, HIGH);

  // Set door status depending on which pin is pressed
  doorStatus = digitalRead(ROOF_PIN);
}

void loop()
{
  // Sleep for 1 minute
  delay(500);

  sensorValue = analogRead(A0);

  //if it is daylight, & the last action is NOT opening, open the door
  if (sensorValue >= LIGHT_THRESHOLD && doorStatus == DOWN){
    moveDoor(UP);
    doorStatus = UP;
    // Now we sleep for some time to prevent 'flickering'
    delay(500);
    lowPowerSleep(SLEEPTIME);
  }

  //if it is night, & the last action is NOT closing, close the door
  if (sensorValue <= DARK_THRESHOLD && doorStatus == UP){
    moveDoor(DOWN);
    doorStatus = DOWN;
    // Now we sleep for some time to prevent 'flickering'
    delay(500);
    lowPowerSleep(SLEEPTIME);
  }
}

// Idea is to turn on the motor in the correct direction
// Then keep polling to see if the button is pressed or the time is up

void moveDoor(boolean direction){
  byte pin;
  if (direction == DOWN)
    pin = FLOOR_PIN;
  if (direction == UP)
    pin = ROOF_PIN;

  analogWrite(ENABLE_PIN, SPEED);
  digitalWrite(MOTOR_PIN_1, !direction);
  digitalWrite(MOTOR_PIN_2, direction);
  while(!digitalRead(pin)){
    (void)0;
  }
  analogWrite(ENABLE_PIN, 0);
}

void lowPowerSleep(int n){
  // Call the LowPower.idle() function n times
  // This allows us to have low-power idle for
  // more than the pre-defined timings
  int i;
  for (i = 0; i < n; i++){
    LowPower.idle(SLEEP_1S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF,
                  SPI_OFF, USART0_OFF, TWI_OFF);
  }
}

