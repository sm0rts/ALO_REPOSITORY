#include <RunningAverage.h>
static float Vb_desired[3] = {1000,0,0};
static float Q_observed[4] = {3,4,5,6};
static float Q_desired[4] = {20,-8,14,25};
static float Velocity_control_output[4] = {0,0,0,0};
const float l = 0.589;
const float w = 0.620;
const float M1C = (l+w)/4;
const float H_0[4][3] = {
  {-M1C , 1 , -1 },
  { M1C , 1 ,  1 },
  { M1C , 1 , -1 },
  {-M1C , 1 ,  1 }
};
const float Q_Kp = 1;
const float Q_Kd = 0.2;
const float Q_Ki = 0.1;
const char Q_buffer_size= 10;
float Q_error[4];
float Q_error_accumulator[4];
float Q_error_derivative[4];
RunningAverage Q_buffer[4] = {
  RunningAverage(Q_buffer_size),
  RunningAverage(Q_buffer_size),
  RunningAverage(Q_buffer_size),
  RunningAverage(Q_buffer_size)};

void get_Q_desired(){
  for (int i = 0; i < 4; i++){
    Q_desired[i] = 0;
    for (int j = 0; j < 3; j++){
      Q_desired[i]= 
      Q_desired[i]+
      H_0[i][j]*Vb_desired[j];
    }
  }
}
void VELOCITY_CONTROL(){
  for(int motor = 0; motor<4;motor++){
    Q_error[motor] = Q_desired[motor]-Q_observed[motor];
    Q_error_accumulator[motor] = Q_error[motor]+Q_error_accumulator[motor];
    Q_buffer[motor].addValue(Q_error[motor]);
    Q_error_derivative[motor] = Q_buffer[motor].getAverage();
    Velocity_control_output[motor] = Q_Kp*Q_error[motor]+Q_Ki*Q_error_accumulator[motor]+Q_Kd*Q_error_derivative[motor];
  }
} 

void setup() {Serial.begin(115200);}

void loop() {
  delay(1000);
  get_Q_desired();
  VELOCITY_CONTROL();
  Serial.println("M \tQ_obs \tQ_d \te_Q \ti_e_Q \td_e \tVCO");
  for(int i = 0; i<4;i++){
    Serial.print(i);
    Serial.print("\t");
    Serial.print(Q_observed[i]);
    Serial.print("\t");
    Serial.print(Q_desired[i]);
    Serial.print(i);
    Serial.print("\t");
    Serial.print(Q_error[i]);
    Serial.print("\t");
    Serial.print(Q_error_accumulator[i]);
    Serial.print("\t");
    Serial.print(Q_error_derivative[i]);
    Serial.print("\t");
    Serial.println(Velocity_control_output[i]);
    Q_observed[i]= Q_observed[i]+Velocity_control_output[i];
  }
  Serial.println("");
}
