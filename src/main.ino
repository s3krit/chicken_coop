#include <LowPower.h>

#define DAYLIGHT_THRESHOLD 250
#define MOTOR_DURATION 3000
#define ROOF_PIN 2
#define FLOOR_PIN 3
#define DOWN true
#define UP false
#define SPEED 128

int enablePin = 11;
int in1Pin = 10;
int in2Pin = 9;
const int analogInPin = A0;
const int speed = 128;
boolean direction = true;
boolean prevdirection = false;
int sensorValue = 0;
int i;
boolean doorStatus = DOWN;

void setup()
{
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);
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
  sensorValue = analogRead(analogInPin);

  Serial.print("sensor = ");
  Serial.println(sensorValue);
  delay(500);

  //if it is daylight, & the last action is NOT opening, open the door
  if (sensorValue >= DAYLIGHT_THRESHOLD && doorStatus == DOWN){
    Serial.println("Opening door");
    moveDoor(UP);
    doorStatus = UP;
  }

  //if it is night, & the last action is NOT closing, close the door
  if (sensorValue <= DAYLIGHT_THRESHOLD && doorStatus == UP){
    Serial.println("Closing door");
    moveDoor(DOWN);
    doorStatus = DOWN;
  }
}

// Idea is to turn on the motor in the correct direction
// Then keep polling to see if the button is pressed or the time is up

void moveDoor(boolean direction){
  int pin;
  if (direction == DOWN)
    pin = FLOOR_PIN;
  if (direction == UP)
    pin = ROOF_PIN;

  Serial.print(digitalRead(FLOOR_PIN));
  Serial.print(" ");
  Serial.println(digitalRead(ROOF_PIN));

  analogWrite(enablePin, SPEED);
  digitalWrite(in1Pin, !direction);
  digitalWrite(in2Pin, direction);
  Serial.println(digitalRead(pin));
  while(!digitalRead(pin)){
    (void)0;
  }
  Serial.println("Pin hit! Stopping motors");
  analogWrite(enablePin, 0);
}

