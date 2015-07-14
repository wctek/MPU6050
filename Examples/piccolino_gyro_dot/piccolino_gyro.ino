/* dot_move.ino
============================================
Move a dot using data collected from MPU6050
Written by A.Sardo (wctek.com) March 31st 2015
============================================

dot_move.ino code is placed under the MIT license
Copyright (c) 2015 Alex Sardo

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

// We'll need I2C for the gyro and the display
#include <Wire.h>
#include <Piccolino_OLED.h>

// I2C MPU6050 (gyro/acc) library
#include <MPU6050.h>

// SSD1306 class default I2C address is 0x3C
Piccolino_OLED display; // our OLED object ...

// MPU6050 class default I2C address is 0x68
MPU6050 accelgyro; // our gyro/acc object

int16_t ax=0, ay=0, az;
int16_t gx, gy, gz;
int16_t axoff,ayoff;


void setup() {

  display.begin();
  display.clear();  
  
  accelgyro.initialize();
    
  // run the gyro once to get the initial offset
  accelgyro.getMotion3(&ay, &ax, &az);
  axoff=ax;
  ayoff=ay;
}


int16_t x=63;
int16_t y=32;

void loop () {

   // clear old dot (write 2 dots lines BLACK)
   display.drawLine(x,y,x,y+1,BLACK);
   display.drawLine(x+1,y,x+1,y+1,BLACK);

   accelgyro.getMotion6(&ay, &ax, &az, &gx, &gy, &gz);

   //remove offset
   ax-=axoff;   
   ay-=ayoff;
   
   //make the read values manageable by dividing them a bit
   ax/=200;
   ay/=200;
   
   //invert x axis
   x-=ax;
   //add y axis
   y+=ay;
   
   // keep it within the display size 128x64
   if(x>126) x=126;
   if(x<0) x=0;
   if(y>62) y=62;
   if(y<0) y=0;

   display.drawLine(x,y,x,y+1,WHITE);
   display.drawLine(x+1,y,x+1,y+1,WHITE);   

   //update display 
   display.update();

}
