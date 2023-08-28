#include <RunningAverage.h>
static float Vb_desired[4] = {0,0,0,0};
static float P_observed[4] = {3,4,5};
static float P_desired[4] = {20000,10000,4000};
const float P_Kp = 1;
const float P_Kd = 0.2;
const float P_Ki = 0.1;
const char P_buffer_size= 10;
float P_error[3];
float P_error_accumulator[3];
float P_error_derivative[3];
RunningAverage P_buffer[3] = {
  RunningAverage(P_buffer_size),
  RunningAverage(P_buffer_size),
  RunningAverage(P_buffer_size)};

void POSITION_CONTROL(){
  for(int i = 0; i<3;i++){
    P_error[i] = P_desired[i]-P_observed[i];
    P_error_accumulator[i] = P_error[i]+P_error_accumulator[i];
    P_buffer[i].addValue(P_error[i]);
    P_error_derivative[i] = P_buffer[i].getAverage();
    Vb_desired[i] = P_Kp*P_error[i]+P_Ki*P_error_accumulator[i]+P_Kd*P_error_derivative[i];
  }
} 

void setup() {Serial.begin(115200);}

void loop() {
  delay(1000);
  POSITION_CONTROL();
  Serial.println("V{b} \t\tP_obs \t\t\tP_d \t\t\te_P \t\t i_e_P \t\t d_e \t\t Vb_desired");
  for(int i = 0; i<3;i++){
    Serial.print(i);
    Serial.print("\t\t");
    Serial.print(P_observed[i]);
    Serial.print("\t\t");
    if (i ==2){Serial.print("\t");}
    Serial.print(P_desired[i]);
    if (i ==2){Serial.print("\t");}
    Serial.print("\t\t");
    Serial.print(P_error[i]);
    Serial.print("\t\t");
    Serial.print(P_error_accumulator[i]);
    Serial.print("\t\t");
    Serial.print(P_error_derivative[i]);
    Serial.print("\t\t");
    Serial.println(Vb_desired[i]);
    P_observed[i]= P_observed[i]+Vb_desired[i];
  }
  Serial.println("");
}