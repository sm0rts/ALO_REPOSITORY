
//serial
String myCmd;
String dump;
//I2C
#include "AS5600.h"
#include "Wire.h"
AS5600 as5600; 
int Rotation = 0;
String oldResult ="";
String Result="";
int int_Result = 0;
int int_oldResult = 0;
volatile bool get_encoder_positions_request = 0;
//control
float u_on = 0;    //control input for active period
float u_off = 0;    //control input for active period
int yd = 0;   //desired position
float error;
//PWM
const int sys_clock = 36000000;
const int desiredfrequencyPWM = 100000; //in Hz
const int overflow_value = sys_clock/desiredfrequencyPWM;
int duty_cycle = 0;                     // 0 to 4096
float normalised_duty_cycle = duty_cycle/4096;
volatile bool toggle = 0;
volatile bool toggle2 = 0;
//volatile int y = 0;    //current position (mm)

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
  Timer3.setPeriod(2000); 
  Timer3.setCompare(TIMER_CH1, 1);
  Timer3.attachInterrupt(TIMER_CH1, handler1);

  digitalWrite(PB10, toggle);
  digitalWrite(PB11, !toggle);
}
//PWM timer interrupt routine
void handler2(void) {
    
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
//Get periodic encoder readings
void handler1(void){
  get_encoder_positions_request = 1;
  toggle2 = !toggle2;
  digitalWrite(PC13,toggle2);
}

void get_encoder_poition(void){
  oldResult = Result;
  Result = as5600.readAngle();
  int_Result = Result.toInt();
  int_oldResult = oldResult.toInt();

  if (3572<int_oldResult  &&  int_Result<500){
   Rotation = Rotation +1;
  }
  if (int_oldResult<500   && 3572<int_Result){
    Rotation = Rotation -1;
  }
      
}
//Basic control 
void generate_input(void){
  error = yd-Rotation;
  normalised_duty_cycle = error/500;
  if(normalised_duty_cycle<0.1){
    normalised_duty_cycle = 0;
  }
  if(0.9<normalised_duty_cycle){
    normalised_duty_cycle = 0.9;
  }
  u_on = overflow_value*normalised_duty_cycle;
  u_off = overflow_value*(1-normalised_duty_cycle);
}

void loop() {// constantly checks if there is any serial com
  while(Serial.available()==0){
    
    if(get_encoder_positions_request==1){
      //Serial.println("YAY");
      get_encoder_poition();
      get_encoder_positions_request = 0;
      generate_input();
    }
    
  }
  myCmd = Serial.readStringUntil('\r');
  if (myCmd == "REQ"){
    if (Result != 0){
      Serial.print("Encoder:OK\t");
      Serial.print((int)int_oldResult);
      Serial.print("\t");
      Serial.print((int)int_Result);
       Serial.print("\t");
    }
    else{
      Serial.print("Encoder error\t");
    }
    Serial.print("\tDesired position\t");
    Serial.print((int)yd);
    Serial.print("\tCurrent position\t");
    Serial.print((int)Rotation);
    Serial.print("\tinput duty cycle\t");
    Serial.println(normalised_duty_cycle);
    
  }
  if (myCmd == "ON"){
    digitalWrite(PC13, LOW);
    yd = yd+500;
    Serial.println("Forward command recieved");
  }
  if (myCmd == "OFF"){
    digitalWrite(PC13, HIGH);
    Serial.println("Backward command recieved");
    yd = Rotation;
  }
  else if(myCmd != "ON" && myCmd !="OFF"&& myCmd !="REQ"){
    Serial.print("unknown command\t "+ myCmd);
  }
  //Serial.println(as5600.readAngle());
  while(Serial.available()==1){
    char t = Serial.read();
  }
}
