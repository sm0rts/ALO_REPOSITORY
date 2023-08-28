
int Velocity_control_output[4] = { 0, -0, 0, 10 };
int PWM_input[4][2];
int MOTOR[4][2] = {{PA0,PA1},{PA2,PA3},{PA6,PA7},{PB0,PB1}}; 
int u_max = 1000;
int max_PWM = 64000;
int min_PWM = 8000;
int start_PWM = 100;
void send_motor_cmd() {
  for (int i = 0; i < 4; i++) {
    int PWM = map(abs(Velocity_control_output[i]), 0, u_max, 0, 65535);
    if (start_PWM<PWM < min_PWM) { PWM = min_PWM; };
    if (PWM > max_PWM) { PWM = max_PWM; };
    if (Velocity_control_output[i] > 0) {
      PWM_input[i][0] = PWM;
      PWM_input[i][1] = 0;}
    if (Velocity_control_output[i] < 0) {
      PWM_input[i][0] = 0;
      PWM_input[i][1] = PWM;}
    pwmWrite(MOTOR[i][0], PWM_input[i][0]);
    pwmWrite(MOTOR[i][1], PWM_input[i][1]);
    }
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
void setup()  {Serial.begin(115200);initialise_PWM();}
void loop()   {
  send_motor_cmd(); 
  for(int i =0;i<4;i++){
    Serial.print("Motor");
    Serial.print(i);
    Serial.print(" Phase1:");
    Serial.print(PWM_input[i][0]);
    Serial.print(" phase2:");
    Serial.println(PWM_input[i][1]);
  }
  delay(1000);}