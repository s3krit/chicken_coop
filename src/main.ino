#include <LowPower.h>

// misc defines
#define DOWN true
#define UP false

// Configurables
// Time to sleep after moving door (seconds)
#define SLEEPTIME 600
// Threshold before it is considered 'morning'
#define LIGHT_THRESHOLD 100
// Threshold before it is considered 'night'
#define DARK_THRESHOLD 100

// Pins
#define ROOF_PIN 3
#define FLOOR_PIN 2
#define MOTOR_PIN_1 9
#define MOTOR_PIN_2 10
#define PHOTORESISTOR_PIN A0

int sensorValue = 0;
boolean doorStatus;

void setup()
{
  pinMode(MOTOR_PIN_1, OUTPUT);
  pinMode(MOTOR_PIN_2, OUTPUT);
  pinMode(ROOF_PIN, INPUT);
  pinMode(FLOOR_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(MOTOR_PIN_1, LOW);
  digitalWrite(MOTOR_PIN_2, LOW);

  // Set door status depending on which pin is pressed
  doorStatus = digitalRead(ROOF_PIN);

  Serial.begin(9600);
  Serial.println("Starting up...");
}

void loop()
{
  // Sleep for a second
  delay(1000);

  sensorValue = analogRead(A0);
  Serial.print("Light sensor is ");
  Serial.println(sensorValue);

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
  if (direction == DOWN){
    Serial.println("Closing door");
    digitalWrite(MOTOR_PIN_1, HIGH);
  }
  if (direction == UP){
    Serial.println("Opening door");
    digitalWrite(MOTOR_PIN_2, HIGH);
  }

  // Wait for two seconds to clear the switch
  delay(2000);

  byte roofSwitch = LOW;
  byte floorSwitch = LOW;
  while(1){
    roofSwitch = digitalRead(ROOF_PIN);
    floorSwitch = digitalRead(FLOOR_PIN);
    if (roofSwitch == HIGH || floorSwitch == HIGH){
      Serial.print("Stopping door because ");
      if (roofSwitch == HIGH){
        Serial.print("roof");
      } else {
        Serial.print("floor");
      }
      Serial.println(" switch is closed");
      break;
    }
  }

  digitalWrite(MOTOR_PIN_1, LOW);
  digitalWrite(MOTOR_PIN_2, LOW);
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
