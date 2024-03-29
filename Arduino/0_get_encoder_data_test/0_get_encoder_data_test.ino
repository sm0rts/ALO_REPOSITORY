#include "AS5600.h"
#include "Wire.h"
#include <RunningAverage.h>
#include <ALO.h>
AS5600 as5600; 

String myCmd;
String str_y; 
static int enc_now[4] =      {0,0,0,0};//current output 
static int enc_last[4] =       {0,0,0,0};//current output  
static int Rotation[4] =      {0,0,0,0};
static int delta_enc[4] =     {0,0,0,0};
static float Q_observed[4] =  {0,0,0,0};
const char Q_buffer_size= 40;
const float R_Q= 37.38;
RunningAverage Q_buffer[4]={
  RunningAverage(Q_buffer_size),
  RunningAverage(Q_buffer_size),
  RunningAverage(Q_buffer_size),
  RunningAverage(Q_buffer_size) };
volatile int control_counter = 0;
volatile bool encoder_read_request = 0;


//motor send command test code
int velocity_control_output[4] = { 9000, -10000, 40000, 10 };
int PWM_input[4][2];
int MOTOR[4][2] = {{PA0,PA1},{PA2,PA3},{PA6,PA7},{PB0,PB1}}; 
int u_max = 400;
int max_PWM = 64000;
int min_PWM = 8000;
int p=0;

void initialise_encoders(){
  Wire.beginTransmission(0x70);
  as5600.begin(4); 
  as5600.setDirection(AS5600_CLOCK_WISE);
  Timer1.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
  //Timer1.setPrescaleFactor(4);
  Timer1.setPeriod(500); // in microseconds
  Timer1.setCompare(TIMER_CH1, 1);      // overflow might be small
  Timer1.attachInterrupt(TIMER_CH1, get_encoder_positions_interrupt);
  for(int i=0;i<4;i++){Q_buffer[i].clear();}
}
void initialise_PWM() {
  pinMode(MOTOR[0][0], PWM);  pinMode(MOTOR[0][1], PWM);
  pinMode(MOTOR[1][0], PWM);  pinMode(MOTOR[1][1], PWM);
  pinMode(MOTOR[2][0], PWM);  pinMode(MOTOR[2][1], PWM);
  pinMode(MOTOR[3][0], PWM);  pinMode(MOTOR[3][1], PWM);
  //default all motors to off
  pwmWrite(MOTOR[0][0], 0);   pwmWrite(MOTOR[0][1], 0);
  pwmWrite(MOTOR[1][0], 0);   pwmWrite(MOTOR[1][1], 0);
  pwmWrite(MOTOR[2][0], 0);   pwmWrite(MOTOR[2][1], 0);
  pwmWrite(MOTOR[3][0], 0);   pwmWrite(MOTOR[3][1], 0);
}


void send_motor_cmd() {
  for (int i = 0; i < 4; i++) {
    int PWM = map(abs(velocity_control_output[i]), 0, u_max, 0, 65535);  //get input amplitude
    if (PWM < min_PWM && velocity_control_output[i] != 0) { PWM = min_PWM; };
    if (PWM > max_PWM) { PWM = max_PWM; };
    //Make sure only 1 phase active at a time
    if (velocity_control_output[i] > 0) {
      PWM_input[i][0] = PWM;
      PWM_input[i][1] = 0;
    }
    if (velocity_control_output[i] < 0) {
      PWM_input[i][0] = 0;
      PWM_input[i][1] = PWM;
    }
    pwmWrite(MOTOR[i][0], PWM_input[i][0]);
    pwmWrite(MOTOR[i][1], PWM_input[i][1]);
    Serial.print("Motor");
    Serial.print(i);
    Serial.print(" Phase1:");
    Serial.print(PWM_input[i][0]);
    Serial.print(" phase2:");
    Serial.println(PWM_input[i][1]);
    }
}

void get_encoder_data(){
  for (int encoder_n = 0; encoder_n < 3;encoder_n++){
    TCA9548A(encoder_n+4);
    enc_last[encoder_n]  = enc_now[encoder_n];
    str_y                = as5600.readAngle();
    enc_now[encoder_n]   = str_y.toInt();
    if (3572< enc_last[encoder_n]  &&   enc_now[encoder_n]<500){
      Rotation[encoder_n] ++;
      delta_enc[encoder_n] = 4096 -  enc_last[encoder_n] +  enc_now[encoder_n];}
    else if (enc_last[encoder_n]<500   && 3572< enc_now[encoder_n]){
      Rotation[encoder_n] --;
      delta_enc[encoder_n] =   enc_now[encoder_n]- enc_last[encoder_n]-4096;}
    else {
      delta_enc[encoder_n] =  enc_now[encoder_n] -  enc_last[encoder_n];}
    Q_observed[encoder_n] = delta_enc[encoder_n]*R_Q;
    Q_buffer[encoder_n].addValue(Q_observed[encoder_n]);
  }
}

void get_encoder_positions_interrupt(void){
 encoder_read_request = 1;
 control_counter++;
}


void setup() {
  Serial.begin(115200);
  initialise_encoders();
  initialise_PWM();
  send_motor_cmd();
}
void loop() {
  if(encoder_read_request == 1){
    get_encoder_data();
    Serial.print(control_counter);
      for(int i=0;i<4;i++){
      if (control_counter>=200){
      Serial.print( Q_buffer[i].getAverage());
      Serial.print("\t");
      }
      Serial.println("");
      control_counter=0;
    }
    encoder_read_request=0;
    }
}