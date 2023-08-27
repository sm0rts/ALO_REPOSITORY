//Serial 
String myCmd;
//Control
static int Rotation[4] = {0,0,0,0};
String str_y[4]={""};
static int  y[4] = {0,0,0,0};//current output 
static int  y_1[4] = {0,0,0,0};//current output  
static int  y_2[4] = {0,0,0,0};//current output
static int  velocity_control_output[4] = {0,0,0,0};
static bool dir_motor[4]= {0,0,0,0};
static int observed_angular_velocity[4] = {0,0,0,0};
static int desired_angular_velocity[4] = {0,0,0,0};
static int error[4]= {0,0,0,0};
const int max_angular_velocity = 200;
const int min_angular_velocity = 50;
const int Kp = 1;
const int u_max = 400;
volatile bool encoder_read_request = 0;
const char Buffer_Size= 400;


const int sys_clock = 36000000;
const int desired_encoder_check_Hz = 100000; //in Hz
const int overflow_value_for_full_period = sys_clock/desired_encoder_check_Hz;   

volatile bool toggle[4] = {0};
static int PWM_input[4][2] = {0};
volatile int control_counter = 0;
int MOTOR[4][2] = {{PA0,PA1},{PA2,PA3},{PA6,PA7},{PB0,PB1}}; 
int chosen_motor =10;
const int max_PWM = 62000;
const int min_PWM = 10000;

void change_motor_variables(int motor){
  desired_angular_velocity[motor] = desired_angular_velocity[motor]+5;
  toggle[motor] = !toggle[motor];
}

void process_motor_command(){
  myCmd = Serial.readStringUntil('\r'); 
  if (myCmd == "REQ"){
    for (int i = 0; i < 4; i++){    
      Serial.print("MOTOR");
      Serial.print(i);
      Serial.print(" vd=");
      Serial.print((int)desired_angular_velocity[i]);
      Serial.print(" v=");
      Serial.print((int)observed_angular_velocity[i]);
      Serial.print(" u1=");
      Serial.print((int)PWM_input[i][0]);
      Serial.print(" u2=");
      Serial.print((int)PWM_input[i][1]);
      Serial.print("\t"); 
      }
    }
  if (myCmd == "UP")    {chosen_motor = 0;}
  if (myCmd == "DOWN")  {chosen_motor = 1;}
  if (myCmd == "LEFT")  {chosen_motor = 2;}
  if (myCmd == "RIGHT") {chosen_motor = 3;}
  if (chosen_motor<=3){
    Serial.print(myCmd);
    change_motor_variables(chosen_motor);
    chosen_motor = 10;
    }
  while(Serial.available()==1){
  char t = Serial.read();
  }
  Serial.println("");
}

void setup() {Serial.begin(115200);}

void loop() {
  if(Serial.available()==1){process_motor_command();}
}
