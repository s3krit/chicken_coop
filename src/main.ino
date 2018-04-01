#include <LowPower.h>

// misc defines
#define DOWN true
#define UP false

// configurables
#define SPEED 128
#define SLEEPTIME 1000
#define LIGHT_THRESHOLD 250
#define DARK_THRESHOLD 250
#define MOTOR_DURATION 3000

// Pins
#define ROOF_PIN 2
#define FLOOR_PIN 3
#define ENABLE_PIN 11
#define MOTOR_PIN_1 10
#define MOTOR_PIN_2 9
#define PHOTORESISTOR_PIN A0

int sensorValue = 0;
int i;
boolean doorStatus = DOWN;

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
  Serial.begin(9600);
  Serial.println("Initiated");
}

void loop()
{
  //LowPower.powerDown(SLEEP_8S, ADC_ON, BOD_ON);

  // Not sure why this is necessary
  /*
  for(i = 0; i < 10; i++){
    LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF,
                  SPI_OFF, USART0_OFF, TWI_OFF);
  }
  */
  //delay(20);
  sensorValue = analogRead(PHOTORESISTOR_PIN);

  Serial.print("sensor = ");
  Serial.println(sensorValue);
  delay(500);

  //if it is daylight, & the last action is NOT opening, open the door
  if (sensorValue >= LIGHT_THRESHOLD && doorStatus == DOWN){
    Serial.println("Opening door");
    moveDoor(UP);
    doorStatus = UP;
    // Now we sleep for some time to prevent 'flickering'
    delay(SLEEPTIME);
  }

  //if it is night, & the last action is NOT closing, close the door
  if (sensorValue <= DARK_THRESHOLD && doorStatus == UP){
    Serial.println("Closing door");
    moveDoor(DOWN);
    doorStatus = DOWN;
    // Now we sleep for some time to prevent 'flickering'
    delay(SLEEPTIME);
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

  Serial.print(digitalRead(FLOOR_PIN));
  Serial.print(" ");
  Serial.println(digitalRead(ROOF_PIN));

  analogWrite(ENABLE_PIN, SPEED);
  digitalWrite(MOTOR_PIN_1, !direction);
  digitalWrite(MOTOR_PIN_2, direction);
  Serial.println(digitalRead(pin));
  while(!digitalRead(pin)){
    (void)0;
  }
  Serial.println("Pin hit! Stopping motors");
  analogWrite(ENABLE_PIN, 0);
}

