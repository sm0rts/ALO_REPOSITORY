#ifndef ALO_H

#define ALO_H
#include "Wire.h"
#include <Arduino.h>

void TCA9548A(uint8_t bus);
int generate_input(char motor,int yd,int y);


#endif