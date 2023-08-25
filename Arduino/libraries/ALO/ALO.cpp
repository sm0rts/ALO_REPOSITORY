#include "ALO.h"


void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
  //Serial.print(bus);
}

int generate_input(char motor,int yd,int y){
  //y_x(k)= a1y(k-1)+a2y(k-2)+b1u(k)+b2u(k-1)+b3u(k-3)
  //y_y(k)= a1y(k-1)+a2y(k-2)+b1u(k)+b2u(k-1)+b3u(k-3)
  //y_R(k)= a1y(k-1)+a2y(k-2)+b1u(k)+b2u(k-1)+b3u(k-3)
  //gets y,y1,y2,u,u1,u2
  //multiplies them by some gains a0,a1,a2,b0,b1,b2 to get control input
  //maps input to PWM (0:65535)

  int error = yd-y;
  int u = error;
  return u;
}

