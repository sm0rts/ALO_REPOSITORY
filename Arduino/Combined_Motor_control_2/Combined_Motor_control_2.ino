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
static int  y_1[4] = {0,0,0,0};//current output  
static int  y_2[4] = {0,0,0,0};//current output   
static int observed_angular_velocity[4] = {0,0,0,0};
static int desired_angular_velocity[4] = {0,0,0,0};
static int error[4]= {0,0,0,0};
const int max_angular_velocity = 200;
const int min_angular_velocity = 50;
const int Kp = 1;
volatile bool encoder_read_request = 0;
const char Buffer_Size= 400;
RunningAverage speedBuffer[4] = {
  RunningAverage(Buffer_Size),
  RunningAverage(Buffer_Size),
  RunningAverage(Buffer_Size),
  RunningAverage(Buffer_Size)};

const int sys_clock = 36000000;
const int desired_encoder_check_Hz = 100000; //in Hz
const int overflow_value_for_full_period = sys_clock/desired_encoder_check_Hz;   

volatile bool toggle[4] = {0};
static int PWM_input[4][2] = {0};
volatile int control_cooldown_counter = 0;
int MOTOR[4][2] = {{PA0,PA1},{PA2,PA3},{PA6,PA7},{PB0,PB1}}; 
int chosen_motor =10;

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
  
  pinMode(MOTOR[0][0], PWM);
  pinMode(MOTOR[0][1], PWM);
  pinMode(MOTOR[1][0], PWM);
  pinMode(MOTOR[1][1], PWM);
  pinMode(MOTOR[2][0], PWM);
  pinMode(MOTOR[2][1], PWM);
  pinMode(MOTOR[3][0], PWM);
  pinMode(MOTOR[3][1], PWM);
  
  pwmWrite(MOTOR[0][0], 0);
  pwmWrite(MOTOR[0][1], 0);
  pwmWrite(MOTOR[1][0], 0);
  pwmWrite(MOTOR[1][1], 0);
  pwmWrite(MOTOR[2][0], 0);
  pwmWrite(MOTOR[2][1], 0);
  pwmWrite(MOTOR[3][0], 0);
  pwmWrite(MOTOR[3][1], 0);
  
  //Timer interrupts for collecting encoder data 
  Timer1.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
  //Timer1.setPrescaleFactor(2);
  Timer1.setPeriod(overflow_value_for_full_period); // in microseconds
  Timer1.setCompare(TIMER_CH1, 1);      // overflow might be small
  Timer1.attachInterrupt(TIMER_CH1, get_encoder_positions_interrupt);
  //empty observed_angular_velocity buffer
  speedBuffer[0].clear();
  speedBuffer[1].clear();
  speedBuffer[2].clear();
  speedBuffer[3].clear();
}

void get_control_inputs_interrupt(){
}

void get_encoder_positions_interrupt(void){
 encoder_read_request = 1;
 control_cooldown_counter++;
}


int velocity_control(int motor){
  observed_angular_velocity[motor] = get_speed(motor);
  int error = desired_angular_velocity[motor]-observed_angular_velocity[motor];
  int u = abs(Kp*error);
  if (u>390){u = 390;}
  int PWM = map(u,0,400,0,65535);
  if (error>0 ){PWM_input[motor][0] = PWM; PWM_input[motor][1] = 0;}
  if (error<=0){PWM_input[motor][1] = PWM; PWM_input[motor][0] = 0;}
}
  /*
  if(u<min_angular_velocity && desired_angular_velocity[motor]!=0 ){
    u=min_angular_velocity;
  }
  if(max_angular_velocity<u){
    u= max_angular_velocity;
  }
  */
  

void process_motor_command(int motor){
      desired_angular_velocity[motor] = desired_angular_velocity[motor]+5;
      toggle[motor] = !toggle[motor];
      /*
      if(toggle[motor] == 1){
        pwmWrite(MOTOR[motor][1], 10000);
        pwmWrite(MOTOR[motor][0], 0);
      }
      if(toggle[motor] == 0){
        pwmWrite(MOTOR[motor][0], 10000);
        pwmWrite(MOTOR[motor][1], 0);
      }
      Serial.write(toggle[motor]);
      */
}

void readCmd(){
  myCmd = Serial.readStringUntil('\r'); 
  if (myCmd == "REQ"){
    for (int i = 4; i <= 7; i++){    
      Serial.print("MOTOR");
      Serial.print(i-4);
      Serial.print(" vd=");
      Serial.print((int)desired_angular_velocity[i-4]);
      Serial.print(" v=");
      Serial.print((int)get_speed(i-4));
      Serial.print(" u1=");
      Serial.print((int)PWM_input[i-4][0]);
      Serial.print(" u2=");
      Serial.print((int)PWM_input[i-4][1]);
      Serial.print("\t"); 
      }
    }
  if (myCmd == "UP")    {chosen_motor = 0;}
  if (myCmd == "DOWN")  {chosen_motor = 1;}
  if (myCmd == "LEFT")  {chosen_motor = 2;}
  if (myCmd == "RIGHT") {chosen_motor = 3;}
  if (chosen_motor<=3){
    Serial.print(myCmd);
    process_motor_command(chosen_motor);
    chosen_motor = 10;
    }
  while(Serial.available()==1){
  char t = Serial.read();
  }
  Serial.println("");
}


void get_encoder_poition(int encoder_n){
   y_2[encoder_n] =  y_1[encoder_n];
   y_1[encoder_n] =  y[encoder_n];
  str_y[encoder_n]       = as5600.readAngle();
   y[encoder_n]   = str_y[encoder_n].toInt();
  if (3572< y_1[encoder_n]  &&   y[encoder_n]<500){
    Rotation[encoder_n] ++;
    observed_angular_velocity[encoder_n] = 4096 -  y_1[encoder_n] +  y[encoder_n];
  }
  else if ( y_1[encoder_n]<500   && 3572< y[encoder_n]){
    Rotation[encoder_n] --;
    observed_angular_velocity[encoder_n] =   y[encoder_n]- y_1[encoder_n]-4096;
  }
  else {
    observed_angular_velocity[encoder_n] =  y[encoder_n] -  y_1[encoder_n];
  }
  speedBuffer[encoder_n].addValue(observed_angular_velocity[encoder_n]);
}

int get_speed(int encoder_n){ 
  return(speedBuffer[encoder_n].getAverage());
}


void loop() {// constantly checks if there is any serial com
  while(Serial.available()==0){
    if(encoder_read_request == 1){
      for (int i = 0; i < 4; i++){
        TCA9548A(i+4);
        get_encoder_poition(i);
      }
      if (control_cooldown_counter>=50){
        control_cooldown_counter = 0;
        for (int i = 0; i < 3; i++){
          velocity_control(i);
          pwmWrite(MOTOR[i][1],PWM_input[i][0]);
          pwmWrite(MOTOR[i][0],PWM_input[i][1]);
          //Serial.print(velocity_control(i-4));
        }
      }
      encoder_read_request=0;
    }
  }
  readCmd();
}
  