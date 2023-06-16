#include <AS5600.h>

volatile unsigned int temp, counter = 9000;
int toggle = 0;
int frequencyPWM = 1;
int period =100/frequencyPWM;
int ton = 10;

void setup() {

  Serial.begin (9600);

  pinMode(PB8, INPUT_PULLUP);
  pinMode(PB9, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PB8), ai1, RISING);
  attachInterrupt(digitalPinToInterrupt(PB9), ai0, RISING);

  pinMode(PC13, OUTPUT);
  pinMode(PB11, OUTPUT);
  pinMode(PB10, OUTPUT);

  Timer2.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
  Timer2.setPeriod(500000); // in microseconds
  Timer2.setCompare(TIMER_CH1, 1);      // overflow might be small
  Timer2.attachInterrupt(TIMER_CH1, handler_PWM);

  digitalWrite(PB10, toggle);
  digitalWrite(PB11, !toggle);
}

void handler_PWM(void) {
    toggle ^= 1;
    digitalWrite(PB10, toggle);
    ton = counter*0.1;
    if(toggle==1){
      Timer2.setPeriod(ton);
    }
    else{
      Timer2.setPeriod(1000-ton);
    }
    
    //+counter*10
}

void ai0() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if(digitalRead(PB8)==LOW) {
  counter++;
  }else{
  counter--;
  }
  }

void ai1() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if(digitalRead(PB9)==LOW) {
  counter--;
  }else{
  counter++;
  }
  }


void loop() {
  Serial.println (ton);
  delay(100);                      // wait for a second                     // wait for a second
}
