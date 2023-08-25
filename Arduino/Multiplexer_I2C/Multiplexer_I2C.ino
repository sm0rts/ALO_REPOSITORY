#include "AS5600.h"
#include "Wire.h"
#include <ALO.h>
AS5600 as5600; 
int Rotation = 0;
String oldResult ="";
String Result="";
int int_Result = 0;
int int_oldResult = 0;
volatile bool get_encoder_positions_request = 0;

void setup() {
  Serial.begin(115200);
  //I2C
  pinMode(PC13, OUTPUT);
  Wire.beginTransmission(0x70);
  as5600.begin(4); 
  as5600.setDirection(AS5600_CLOCK_WISE);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 4; i <= 7; i++){
    TCA9548A(i);
    Serial.println(as5600.readAngle());
    delay(100);
  }
  Serial.println("-------------");
  delay(500);
}
