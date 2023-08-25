#include "AS5600.h"
#include "Wire.h"
#include <RunningAverage.h>
AS5600 as5600; 

//Serial 
String myCmd;
//Control
static int Rotation[4] = {0,0,0,0};
String y_1[4] ={""};
String y[4]={""};
int int_y[4] = {0,0,0,0};//current output 
int int_y_1[4] = {0,0,0,0};//last output
int int_y_2[4] = {0,0,0,0};//2nd last output
int int_u[4] = {0,0,0,0};//current input 
int int_u_1[4] = {0,0,0,0};//last input 
int int_u_2[4] = {0,0,0,0};//2nd last input 
int speed[4] = {0,0,0,0};
volatile bool serial_write_request = 0;
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
volatile bool toggle = 0;
volatile int kk = 0;

int FL1 = PA0;
int FL2 = PA1;
int BL1 = PA2;
int BL2 = PA3;
int FR1 = PA6;
int FR2 = PA7;
int BR1 = PB0;
int BR2 = PB1;


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
  
  pwmWrite(FL1, 50000);
  pwmWrite(FL2, 0);
  
  pwmWrite(BL1, 50000);
  pwmWrite(BL2, 0);
  
  pwmWrite(FR1, 50000);
  pwmWrite(FR2, 0);
  
  pwmWrite(BR1, 50000);
  pwmWrite(BR2, 0);
  
  //Timer interrupts for collecting encoder data 
  Timer1.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
  Timer1.setPeriod(200000); // in microseconds
  Timer1.setCompare(TIMER_CH1, 1);      // overflow might be small
  Timer1.attachInterrupt(TIMER_CH1, get_encoder_positions_interrupt);
  //empty speed buffer
  speedBuffer[0].clear();
  speedBuffer[1].clear();
  speedBuffer[2].clear();
  speedBuffer[3].clear();
}

void get_encoder_positions_interrupt(void){
 digitalWrite(PC13, !digitalRead(PC13));
 serial_write_request =1;
}

void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
  //Serial.print(bus);
}

void get_encoder_poition(int encoder_n){
  int_y_2[encoder_n] = int_y_1[encoder_n];
  int_y_1[encoder_n] = int_y[encoder_n];
  y[encoder_n]       = as5600.readAngle();
  int_y[encoder_n]   = y[encoder_n].toInt();
  if (3572<int_y_1[encoder_n]  &&  int_y[encoder_n]<500){
    Rotation[encoder_n] ++;
    speed[encoder_n] = 4096 - int_y_1[encoder_n] + int_y[encoder_n];
  }
  else if (int_y_1[encoder_n]<500   && 3572<int_y[encoder_n]){
    Rotation[encoder_n] --;
    speed[encoder_n] =  int_y[encoder_n]-int_y_1[encoder_n]-4096;
  }
  else {
    speed[encoder_n] = int_y[encoder_n] - int_y_1[encoder_n];
  }
  speedBuffer[encoder_n].addValue(speed[encoder_n]);

}

int get_speed(int encoder_n){ 
  return(speedBuffer[encoder_n].getAverage());
}

void loop() {// constantly checks if there is any serial com
  while(Serial.available()==0){
    for (int i = 4; i <= 7; i++){
      TCA9548A(i);
      get_encoder_poition(i-4);
    }

    if(serial_write_request == 1){
      
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
      Serial.println("");
    }
    if (myCmd == "ON"){
      Serial.println("Forward command recieved");
    }
    if (myCmd == "OFF"){
      Serial.println("Backward command recieved");
    }
    if (myCmd == "STOP"){
      Serial.println("STOP command recieved");
    }
    while(Serial.available()==1){
    char t = Serial.read();
  }

}
    
  