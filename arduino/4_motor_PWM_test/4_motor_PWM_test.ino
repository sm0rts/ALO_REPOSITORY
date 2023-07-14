#include "AS5600.h"
#include "Wire.h"
AS5600 as5600; 
int Rotation = 0;
String oldResult ="";
String Result="";
int int_Result = 0;
int int_oldResult = 0;
volatile bool get_encoder_positions_request = 0;

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
  Serial.begin(9600);
  Serial.println("-------------");
  //I2C
  
  pinMode(PC13, OUTPUT);
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
  pwmWrite(FL1, 15000);
  pwmWrite(FL2, 0);
  pwmWrite(BL1, 30000);
  pwmWrite(BL2, 0);
  pwmWrite(FR1, 45000);
  pwmWrite(FR2, 0);
  pwmWrite(BR1, 60000);
  pwmWrite(BR2, 0);
  //Timer interrupts for collecting encoder data 
  Timer1.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
  Timer1.setPeriod(50000); // in microseconds
  Timer1.setCompare(TIMER_CH1, 1);      // overflow might be small
  Timer1.attachInterrupt(TIMER_CH1, PWM_BL);
  
}

void PWM_BL(void){
 digitalWrite(PC13, !digitalRead(PC13));
 kk++;
 if (kk == 50){
   get_encoder_positions_request = 1; 
   kk= 0;
 }
}
void PWM_DL(void){
 kk++;
}
void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
  //Serial.print(bus);
}

void get_encoder_poition(void){
  oldResult = Result;
  Result = as5600.readAngle();
  int_Result = Result.toInt();
  int_oldResult = oldResult.toInt();

  if (3572<int_oldResult  &&  int_Result<500){
   Rotation = Rotation +1;
  }
  if (int_oldResult<500   && 3572<int_Result){
    Rotation = Rotation -1;
  }
}

void loop() {// constantly checks if there is any serial com
  get_encoder_poition();
  Serial.print("Rotaions:\t");
  Serial.print((int)kk);
  Serial.print("\tOld Result:\t");
  Serial.print((int)int_oldResult);
  Serial.print("\tResult:\t");
  Serial.println((int)int_Result);
  while(Serial.available()==1){
    char t = Serial.read();
  }
  delay(500);
  
}
