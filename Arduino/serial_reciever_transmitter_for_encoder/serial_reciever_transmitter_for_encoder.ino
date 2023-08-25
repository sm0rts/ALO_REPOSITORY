#include "AS5600.h"
#include "Wire.h"

String myCmd;
String oldResult ="";
String Result="";

AS5600 as5600; 
int Rotation = 0;
int int_Result = 0;
int int_oldResult = 0;

void setup() {
  //serial com setup
  Serial.begin(115200);
  as5600.begin(4); 
  pinMode(PC13, OUTPUT);
  as5600.setDirection(AS5600_CLOCK_WISE);
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
  while(Serial.available()==0){
      //loop while no data
  }
  myCmd = Serial.readStringUntil('\r');
  if (myCmd == "ON"){
    digitalWrite(PC13, LOW);
    Serial.print("LED is ON\t encoder:");
  }
  if (myCmd == "OFF"){
    digitalWrite(PC13, HIGH);
    Serial.print("LED is OFF\t encoder:");
  }
  else if(myCmd != "ON" && myCmd !="OFF"&& myCmd !="REQ"){
    Serial.print("unknown command\t "+ myCmd);
  }
  get_encoder_poition();
  Serial.print("Rotaions:\t");
  Serial.print((int)Rotation);
  Serial.print("\tOld Result:\t");
  Serial.print((int)int_oldResult);
  Serial.print("\tResult:\t");
  Serial.println((int)int_Result);
  while(Serial.available()==1){
    char t = Serial.read();
  }
  
}
