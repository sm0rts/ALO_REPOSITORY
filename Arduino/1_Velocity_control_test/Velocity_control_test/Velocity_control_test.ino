#include <RunningAverage.h>
static float  velocity_control_output[4] = {0,0,0,0};
static bool dir_motor[4]= {0,0,0,0};
static float observed_angular_velocity[4] = {3,4,5,6};
static float desired_angular_velocity[4] = {20,-8,14,25};
const float Kp = 1;
const float Kd = 0.2;
const float Ki = 0.1;
const char angular_velocity_buffer= 10;
float error[4];
float error_accumulator[4];
float derivative_error[4];
RunningAverage angular_velocity_error_buffer[4] = {
  RunningAverage(angular_velocity_buffer),
  RunningAverage(angular_velocity_buffer),
  RunningAverage(angular_velocity_buffer),
  RunningAverage(angular_velocity_buffer)};


void velocity_control(){
  for(int motor = 0; motor<4;motor++){
    error[motor] = desired_angular_velocity[motor]-observed_angular_velocity[motor];
    error_accumulator[motor] = error[motor]+error_accumulator[motor];
    angular_velocity_error_buffer[motor].addValue(error[motor]);
    derivative_error[motor] = angular_velocity_error_buffer[motor].getAverage();
    velocity_control_output[motor] = Kp*error[motor]+Ki*error_accumulator[motor]+Kd*derivative_error[motor];
  }
} 

void setup() {Serial.begin(115200);}

void loop() {
  delay(1000);
  velocity_control();
  Serial.println("M \tobs \tdes \te \ti_e \td_e \tvc");
  for(int i = 0; i<4;i++){
    Serial.print(i);
    Serial.print("\t");
    Serial.print(observed_angular_velocity[i]);
    Serial.print("\t");
    Serial.print(desired_angular_velocity[i]);
    Serial.print(i);
    Serial.print("\t");
    Serial.print(error[i]);
    Serial.print("\t");
    Serial.print(error_accumulator[i]);
    Serial.print("\t");
    Serial.print(derivative_error[i]);
    Serial.print("\t");
    Serial.println(velocity_control_output[i]);
    observed_angular_velocity[i]= observed_angular_velocity[i]+velocity_control_output[i];
  }
  Serial.println("");
}
