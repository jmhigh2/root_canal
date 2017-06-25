#define FREQUENCY 490     // in HZ
#define CLOCK_PIN 6 //output clock to motor driver
#define DIRECTION_PIN 7
#define STEP_NUMBER 200

#define SINGLE_STEP_BUTTON 4
#define CONTINUOUS_STEP_BUTTON 5

#define FORWARD_STOP 2
#define RETRACT_STOP 3

volatile int state = 0;
volatile int nowinter1, previnter1;
volatile int nowinter2, previnter2;

const int CLOCK_DELAY = 1000000/FREQUENCY/2;

void setup() {
  // put your setup code here, to run once:
  pinMode(DIRECTION_PIN, OUTPUT); //direction pin
  pinMode(CLOCK_PIN, OUTPUT); //pwm pin
  
  pinMode(SINGLE_STEP_BUTTON, INPUT);
  pinMode(CONTINUOUS_STEP_BUTTON, INPUT);

  pinMode(FORWARD_STOP, INPUT);
  pinMode(RETRACT_STOP, INPUT);

  attachInterrupt(digitalPinToInterrupt(2), retract, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), stopretract, FALLING);  

  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(DIRECTION_PIN, LOW);

  delay(3000);

}

void loop() {
  // put your main code here, to run repeatedly:
  //stepMotor();

  
  switch(state) {
    case 0: //moving forward
      if (single_pressed()) {
        stepSingle();
        delay(1000);
      }

      else if (continuous_pressed()) {
        stepMotor();
      }
      
    break;
    case 1: //retracting backward
         stepMotor();
    break;
    
  }
  

}

void stepSingle() {

  for (int i = STEP_NUMBER; i > 0; i--) {
      digitalWrite(CLOCK_PIN, HIGH);
      delayMicroseconds(CLOCK_DELAY);
      digitalWrite(CLOCK_PIN, LOW);
      delayMicroseconds(CLOCK_DELAY);
  }
  
}

void stepMotor() {

  int period = 1000000/FREQUENCY/2;
  
      digitalWrite(CLOCK_PIN, HIGH);
      delayMicroseconds(CLOCK_DELAY);
      digitalWrite(CLOCK_PIN, LOW);
      delayMicroseconds(CLOCK_DELAY);
}

int single_pressed() {
  static int buttonState = HIGH;
  
  buttonState = digitalRead(SINGLE_STEP_BUTTON);

  return !buttonState;
  
}


int continuous_pressed() {

  static int buttonState = HIGH;
  
  buttonState = digitalRead(CONTINUOUS_STEP_BUTTON);

  return !buttonState;


}


void retract() {

  nowinter1 = millis();

  if (nowinter1 - previnter1 < 500) {
    return;
  }
  
  if (state == 0) {
    state = 1;
    digitalWrite(DIRECTION_PIN, HIGH);
    previnter1 = nowinter1;
  }
}

void stopretract() {

  nowinter2 = millis();

  if (nowinter2 - previnter2 < 500) {
    return;
  }
  
  if (state == 1) {
    state = 0;
    digitalWrite(DIRECTION_PIN, LOW);
    previnter2 = nowinter2;
  }
}

