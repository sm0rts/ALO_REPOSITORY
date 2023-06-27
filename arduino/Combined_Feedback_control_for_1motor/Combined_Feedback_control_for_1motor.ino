
//serial
String myCmd;
String dump;
//I2C
#include "AS5600.h"
#include "Wire.h"
AS5600 as5600; 
int encoder_val = 0;
//PWM
const int sys_clock = 36000000;
const int desiredfrequencyPWM = 100000; //in Hz
const int overflow_value = sys_clock/desiredfrequencyPWM;
int duty_cycle = 0;                     // 0 to 4096
volatile float normalised_duty_cycle = duty_cycle/4096;
int toggle = 0;
//control
volatile int u = 0;    //control input
volatile int y = 0;    //current position (mm)
int yd = 0;   //desired position
volatile float error;


void setup() {
  //serial
  Serial.begin(115200);
  //I2C
  pinMode(PC13, OUTPUT);
  as5600.begin(4); 
  as5600.setDirection(AS5600_CLOCK_WISE);
  //PWM
  pinMode(PB11, OUTPUT);//  forward dir pin
  pinMode(PB10, OUTPUT);//  backward dir pin

  Timer2.setMode(TIMER_CH2, TIMER_OUTPUTCOMPARE);
  Timer2.setPeriod(overflow_value); // in microseconds
  Timer2.setCompare(TIMER_CH2, 1);      // overflow might be small
  Timer2.attachInterrupt(TIMER_CH2, handler2);
  
  Timer3.setMode(TIMER_CH1,TIMER_OUTPUTCOMPARE);
  Timer3.setPeriod(800000); 
  Timer3.setCompare(TIMER_CH1, 1);
  Timer3.attachInterrupt(TIMER_CH1, handler1);
  /*
  Timer3.setOverflow(9000000);
  Timer3.setCompare(TIMER_CH1, 5000); 
  Timer3.attachInterrupt(TIMER_CH1, handler1);
  Timer3.resume();
  */
  digitalWrite(PB10, toggle);
  digitalWrite(PB11, !toggle);
}

void handler2(void) {//PWM timer interrupt routine
    
    //normalised_duty_cycle = normalised_duty_cycle+1;
    
    toggle ^= 1;
    if(toggle==1){//on Cycle
      Timer2.setPeriod(overflow_value*normalised_duty_cycle);
    }
    if(toggle==0){//off Cycle
      Timer2.setPeriod(overflow_value*(1-normalised_duty_cycle));
    }
    if(normalised_duty_cycle == 0){
      Timer2.setPeriod(overflow_value);
      toggle = 1;
    }
    digitalWrite(PB10, toggle);
}


    
void handler1(void){
  //y = y+1;
  if(yd>y){
    y = y+1;
    error = yd - y;
    normalised_duty_cycle = error/10;
  }
  
}


void loop() {// constantly checks if there is any serial com
  while(Serial.available()==0){
      //loop while no data
  }
  myCmd = Serial.readStringUntil('\r');
  if (myCmd == "REQ"){
    //Serial.print("Current position:\t ");
    //y = get_encoder_data
  }
  if (myCmd == "ON"){
    digitalWrite(PC13, LOW);
    //Serial.print("LED is ON\t encoder:");
    yd = yd+10;
  }
  if (myCmd == "OFF"){
    digitalWrite(PC13, HIGH);
    //Serial.print("LED is OFF\t encoder:");
  }
  else if(myCmd != "ON" && myCmd !="OFF"&& myCmd !="REQ"){
    Serial.print("unknown command\t "+ myCmd);
  }
  //Serial.println(as5600.readAngle());
  Serial.print("counter1:\t");
  Serial.print(overflow_value*normalised_duty_cycle);
  Serial.print("\t counter2:\t");
  Serial.println(y);

  while(Serial.available()==1){
    char t = Serial.read();
  }
}
