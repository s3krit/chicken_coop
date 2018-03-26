#include <LowPower.h>

#define DAYLIGHT_THRESHOLD 350
#define MOTOR_DURATION 3000

int enablePin = 11;
int in1Pin = 10;
int in2Pin = 9;
int switchPin = 7;
const int analogInPin = A0;
const int speed = 128;
boolean direction = true;
boolean prevdirection = false;
int sensorValue = 0;
int i;

void setup()
{
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
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

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);

  Serial.print("sensor = ");
  Serial.println(sensorValue);
  delay(500);

  //if it is daylight, & the last action is NOT opening, open the door
  if (sensorValue >= DAYLIGHT_THRESHOLD && prevdirection == direction){
    Serial.println("Opening door");
    setMotor(speed, direction);
    delay(MOTOR_DURATION);
    setMotor(0, direction);
    delay(MOTOR_DURATION);
    //switch direction
    direction = !direction;

  }

  //if it is night, & the last action is NOT closing, close the door
  if (sensorValue <= DAYLIGHT_THRESHOLD && prevdirection != direction){
    Serial.println("Closing door");
    setMotor(speed, direction);
    delay(MOTOR_DURATION);
    setMotor(0, direction);
    delay(MOTOR_DURATION);
  //switch direction
  direction = ! direction;

  }
}

void setMotor(int speed, boolean direction)
{
  analogWrite(enablePin, speed);
  digitalWrite(in1Pin, !direction);
  digitalWrite(in2Pin, direction);
}
