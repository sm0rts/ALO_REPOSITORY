#include "AS5600.h"
#include "Wire.h"
#include <RunningAverage.h>
#include <ALO.h>
AS5600 as5600; 

//Serial 
String myCmd;
//Control
static int Rotation[4] = {0,0,0,0};
String str_y[4]={""};
static int  y[4] = {0,0,0,0};//current output 
static int  y_1[4] = {0,0,0,0};//last output
static int  y_2[4] = {0,0,0,0};//2nd last output
static int  u[4] = {0,0,0,0};//current input 
static int  u_1[4] = {0,0,0,0};//last input 
static int  u_2[4] = {0,0,0,0};//2nd last input 
static int speed[4] = {0,0,0,0};
static int yd[4] = {0,0,0,0};
static int error[4]= {0,0,0,0};
const int max_speed = 200;
const int min_speed = 50;
volatile bool serial_write_request = 0;
//averaging speed to eliminate noise
const char Buffer_Size= 400;
RunningAverage speedBuffer[4] = {RunningAverage(Buffer_Size),RunningAverage(Buffer_Size),
    RunningAverage(Buffer_Size),RunningAverage(Buffer_Size)};

//PWM
const int sys_clock = 36000000;
const int desiredfrequencyPWM = 100000; //in Hz
const int overflow_value_for_full_period = sys_clock/desiredfrequencyPWM;   
const float min_duty_cycle = 0.2;
const float max_duty_cycle = 0.9;             
float duty_cycle_FL = 0;
float duty_cycle_BL = 0;
float duty_cycle_FR = 0;
float duty_cycle_BR = 0;
volatile bool toggle[4] = {0};
volatile int kk = 0;

int FL1 = PA0;
int FL2 = PA1;
int BL1 = PA2;
int BL2 = PA3;
int FR1 = PA6;
int FR2 = PA7;
int BR1 = PB0;
int BR2 = PB1;

//test 
int a = 0;

void setup() {
  //serial
  Serial.begin(115200);
  delay(100);
  Serial.println("-------------");
  //I2C
  
  pinMode(PC13, OUTPUT);
  Wire.beginTransmission(0x70);
  as5600.begin(4); 
  as5600.setDirection(AS5600_CLOCK_WISE);

  //PWM
  
  pinMode(FL1, PWM);
  pinMode(FL2, PWM);
  pinMode(BL1, PWM);
  pinMode(BL2, PWM);
  pinMode(FR1, PWM);
  pinMode(FR2, PWM);
  pinMode(BR1, PWM);
  pinMode(BR2, PWM);
  
  pwmWrite(FL1, 20000);
  pwmWrite(FL2, 0);
  //
  pwmWrite(BL1, 20000);
  pwmWrite(BL2, 0);
  //
  pwmWrite(FR1, 20000);
  pwmWrite(FR2, 0);
  
  pwmWrite(BR1, 20000);
  pwmWrite(BR2, 0);
  
  //Timer interrupts for collecting encoder data 
  Timer1.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
  //Timer1.setPrescaleFactor(2);
  Timer1.setPeriod(4000); // in microseconds
  Timer1.setCompare(TIMER_CH1, 1);      // overflow might be small
  Timer1.attachInterrupt(TIMER_CH1, get_encoder_positions_interrupt);

  //empty speed buffer
  speedBuffer[0].clear();
  speedBuffer[1].clear();
  speedBuffer[2].clear();
  speedBuffer[3].clear();
}

void get_control_inputs_interrupt(){
}

void get_encoder_positions_interrupt(void){
 serial_write_request = 1;
 kk++;
}
/*
int generate_input(char motor,int yd,int y){
  //y_x(k)= a1y(k-1)+a2y(k-2)+b1u(k)+b2u(k-1)+b3u(k-3)
  //y_y(k)= a1y(k-1)+a2y(k-2)+b1u(k)+b2u(k-1)+b3u(k-3)
  //y_R(k)= a1y(k-1)+a2y(k-2)+b1u(k)+b2u(k-1)+b3u(k-3)
  //gets y,y1,y2,u,u1,u2
  //multiplies them by some gains a0,a1,a2,b0,b1,b2 to get control input
  //maps input to PWM (0:65535)

  int error = yd-y;
  int u = error;
  return u;
  
  //limiting duty cycle 
  if(u[motor]<min_speed && Rotation[motor]!=yd[motor] ){
    u[motor]=min_speed;
  }
  if(max_speed<u[motor]){
    u[motor]= max_speed;
  }
  //Setting PWM timings
  
}
*/

void get_encoder_poition(int encoder_n){
   y_2[encoder_n] =  y_1[encoder_n];
   y_1[encoder_n] =  y[encoder_n];
  str_y[encoder_n]       = as5600.readAngle();
   y[encoder_n]   = str_y[encoder_n].toInt();
  if (3572< y_1[encoder_n]  &&   y[encoder_n]<500){
    Rotation[encoder_n] ++;
    speed[encoder_n] = 4096 -  y_1[encoder_n] +  y[encoder_n];
  }
  else if ( y_1[encoder_n]<500   && 3572< y[encoder_n]){
    Rotation[encoder_n] --;
    speed[encoder_n] =   y[encoder_n]- y_1[encoder_n]-4096;
  }
  else {
    speed[encoder_n] =  y[encoder_n] -  y_1[encoder_n];
  }
  speedBuffer[encoder_n].addValue(speed[encoder_n]);
}

int get_speed(int encoder_n){ 
  return(speedBuffer[encoder_n].getAverage());
}

void loop() {// constantly checks if there is any serial com


  while(Serial.available()==0){
    if(serial_write_request == 1){
      for (int i = 4; i <= 7; i++){
        TCA9548A(i);
        get_encoder_poition(i-4);
        a = a+1;
      }
      
      if (kk>=50){
        //Serial.print(kk);
        digitalWrite(PC13, !digitalRead(PC13));
        kk = 0;
        
        for (int i = 4; i <= 7; i++){
          generate_input(i-4, yd[i-4] , y[i-4]);
        }
      }
      serial_write_request=0;
    }
  }



  myCmd = Serial.readStringUntil('\r');
  if (myCmd == "REQ"){
    Serial.print("positions \t");
      for (int i = 4; i <= 7; i++){
        Serial.print((int)Rotation[i-4]);
        Serial.print("\t");
      }
    Serial.print("speeds \t");
      for (int i = 4; i <= 7; i++){
        Serial.print((int)get_speed(i-4));
        Serial.print("\t");
      }
    Serial.print("u \t");
      for (int i = 4; i <= 7; i++){
        Serial.print((int)u[i-4]);
        Serial.print("\t");
      }
      Serial.println("");
    }
    if (myCmd == "LEFT"){
      Serial.println("FL command recieved");
      yd[0] = yd[0]+100;
      toggle[0] = !toggle[0];
      if(toggle[0] == 1){
        pwmWrite(FL1, 40000);
      }
      if(toggle[0] == 0){
        pwmWrite(FL1, 12000);
      }
    }
    if (myCmd == "UP"){
      Serial.println("FR command recieved");
      yd[1] = yd[1]+100;
      toggle[1] = !toggle[1];
      if(toggle[1] == 1){
        pwmWrite(FR1, 40000);
      }
      if(toggle[1] == 0){
        pwmWrite(FR1, 12000);
      }
    }
    if (myCmd == "DOWN"){
      Serial.println("BL command recieved");
      yd[2] = yd[2]+100;
      toggle[2] = !toggle[2];
      if(toggle[2] == 1){
        pwmWrite(BL1, 40000);
      }
      if(toggle[2] == 0){
        pwmWrite(BL1, 12000);
      }
    }
    if (myCmd == "RIGHT"){
      Serial.println("BR command recieved");
      yd[3] = yd[3]+100;
      toggle[3] = !toggle[3];
      if(toggle[3] == 1){
        pwmWrite(BR1, 40000);
      }
      if(toggle[3] == 0){
        pwmWrite(BR1, 12000);
      }
    }
    while(Serial.available()==1){
    char t = Serial.read();
  }
  

}
    
  