String myCmd;
String dump;
#include "AS5600.h"
#include "Wire.h"
AS5600 as5600; 
int encoder_val = 0;

void setup() {
  //serial com setup
  Serial.begin(115200);
  as5600.begin(4); 
  pinMode(PC13, OUTPUT);
  as5600.setDirection(AS5600_CLOCK_WISE);
  //interrupt setup
  Timer2.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
  Timer2.setPeriod(50000000); // in microseconds
  Timer2.setCompare(TIMER_CH1, 1);      // overflow might be small
  Timer2.attachInterrupt(TIMER_CH1, interrupt_routine);
}

void interrupt_routine(void) {//periodically reads the value stored on the encoder
      //Timer2.setPeriod(500);
      //Timer2.setPeriod(1000);
      //digitalWrite(PC13, !digitalRead(PC13));
      //encoder_val = as5600.readAngle();
    }
    

void loop() {// constantly checks if there is any serial com
  while(Serial.available()==0){
      //loop while no data
  }
  myCmd = Serial.readStringUntil('\r');
  if (myCmd == "REQ"){
    Serial.print("Current position:\t ");
  }
  if (myCmd == "ON"){
    digitalWrite(PC13, LOW);
    Serial.print("LED is ON\t encoder:");
    //encoder_val = as5600.readAngle();
    
    //dump = Serial.readStringUntil('\r');
  }
  if (myCmd == "OFF"){
    digitalWrite(PC13, HIGH);
    Serial.print("LED is OFF\t encoder:");
  }
  else if(myCmd != "ON" && myCmd !="OFF"&& myCmd !="REQ"){
    Serial.print("unknown command\t "+ myCmd);
  }
  Serial.println(as5600.readAngle());
  while(Serial.available()==1){
    char t = Serial.read();
  }
}
