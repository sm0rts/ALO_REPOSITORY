#include "AS5600.h"
#include "Wire.h"
#include <RunningAverage.h>
#include <ALO.h>
AS5600 as5600; 
//motor PWM variables
int PWM_input[4][2];
int MOTOR[4][2] = {{PA0,PA1},{PA2,PA3},{PA6,PA7},{PB0,PB1}}; 
int u_max = 1000;
int max_PWM = 64000;
int min_PWM = 8000;
int start_PWM = 100;
//kinematics and wheel variable vectors
static int    P_desired[3]    = {0,0,0};
static int    P_observed[3]   = {0,0,0};
static int    Vb_desired[3]   = {0,0,0};
static int    Vb_observed[3]  = {0,0,0};
static int    Vs_observed[3]  = {0,0,0};
static int    enc_last[4]     = {0,0,0,0};//last output 
static int    enc_now[4]      = {0,0,0,0};//current output  
static int    Rotation[4]     = {0,0,0,0};
static int    delta_enc[4]    = {0,0,0,0};
static float  Q_observed[4]   = {0,0,0,0};
static float  Q_desired[4]    = {0,0,0,0};
static float  Velocity_control_output[4] = { 0, -0, 0, 10 };
static float  phi = 1000;
static float  phi_rad = 0;
//Numerical constants
const float dt = 0.005;
const float l = 0.589;
const float w = 0.620;
const float KH0 = 1/(l+w);
const float M1C = (l+w)/4;
const float R_Q= 37.38; //Q= Ru
//Matrixes
const float H_0_inverse[3][4] ={    { -KH0, KH0, KH0, -KH0 },
                                    {   1,   1,   1,    1  },
                                    {  -1,   1,  -1,    1  },};

float H_Phi[3][3]{                  {1,      0,        0    },
                                    {0,   cos(phi), sin(phi)},
                                    {0,  -sin(phi), cos(phi)}};

const float H_0[4][3] = {           {-M1C , 1 , -1 },
                                    { M1C , 1 ,  1 },
                                    { M1C , 1 , -1 },
                                    {-M1C , 1 ,  1 }};
//Control loop related variables
const char Q_buffer_size= 10;
const char P_buffer_size= 10;
const float Q_Kp = 1; const float Q_Kd = 0.2; const float Q_Ki = 0.1;
const float P_Kp = 1; const float P_Kd = 0.2; const float P_Ki = 0.1;
float Q_error[4]; float Q_error_accumulator[4]; float Q_error_derivative[4];
float P_error[3]; float P_error_accumulator[3]; float P_error_derivative[3];
RunningAverage Q_buffer[4] = {
  RunningAverage(Q_buffer_size),
  RunningAverage(Q_buffer_size),
  RunningAverage(Q_buffer_size),
  RunningAverage(Q_buffer_size)};
RunningAverage P_buffer[3] = {
  RunningAverage(P_buffer_size),
  RunningAverage(P_buffer_size),
  RunningAverage(P_buffer_size)};
//process flow variables
volatile int  control_counter = 0;
volatile bool encoder_read_request = 0;
//communication with Pi
String myCmd;
void up_click_response()    {P_desired[1] = P_desired[1]+1000;}
void down_click_response()  {P_desired[1] = P_desired[1]-1000;}
void left_click_response()  {P_desired[0] = P_desired[0]-1000;}
void right_click_response() {P_desired[0] = P_desired[0]+1000;}
//initialisation functions..........................................................
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
void initialise_encoders(){
  Wire.beginTransmission(0x70);
  as5600.begin(4); 
  as5600.setDirection(AS5600_CLOCK_WISE);
  Timer1.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
  Timer1.setPeriod(1000); // in microseconds
  Timer1.setCompare(TIMER_CH1, 1);      // overflow might be small
  Timer1.attachInterrupt(TIMER_CH1, get_encoder_positions_interrupt);
  Q_buffer[0].clear();
  Q_buffer[1].clear();
  Q_buffer[2].clear();
  Q_buffer[3].clear();
}
void get_encoder_positions_interrupt(void){
 encoder_read_request = 1;
 control_counter++;
}

void setup() {
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:

}
