#define address1 0x36
#define msg_lenght 12
 
#include <Wire.h>

 
void setup()
{
  Wire.begin();
  Serial.begin(9600);
  Serial.println("\nI2C Scanner");
}
 
 
void loop()
{
  Wire.beginTransmission(address1);
  Wire.write(0);
  Wire.endTransmission();
  
  Wire.requestFrom(address1,msg_lenght);
  int response = 0;
  uint8_t i = 0;
  while (Wire.available()) {
      uint8_t b = Wire.read();
      response += 2^i*b;
      i++;
  } 
/*
String response = "";
  while (Wire.available()) {
      char b = Wire.read();
      response += b;
      */

  Serial.println(response);
  delay(200);
}


