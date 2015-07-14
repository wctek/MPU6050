/* gyro_test.ino
============================================
draw 3 tiny graphs using data collected from MPU6050
Written by A.Sardo (wctek.com) March 31st 2015
============================================

gyro_test.ino code is placed under the MIT license
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

char xlog[128];
char ylog[128];
char zlog[128];
char index=1;

void setup() {

  display.begin();
  display.clear();  
  
  accelgyro.initialize();
    
  // run the gyro once to get the initial offset
  accelgyro.getMotion3(&ay, &ax, &az);
  axoff=ax/200;
  ayoff=ay/200;
  
  memset(xlog,0,128);
  memset(ylog,0,128);
  memset(zlog,0,128);
}


int16_t x=63;
int16_t y=32;

void fix(char *what) {
   if(*what>9) *what=9;
   if(*what<-9) *what=-9;
}

void showdata(void) {
  
  for(char f=1; f<126; f++) {
     display.drawLine(f-1,10+xlog[f-1],f,10+xlog[f],WHITE);
     display.drawLine(f-1,31+ylog[f-1],f,31+ylog[f],WHITE);
     display.drawLine(f-1,52+zlog[f-1],f,52+zlog[f],WHITE);
  }
  
  
//  display.drawPixel(60,10+xlog[1],WHITE);
}

void loop () {

   accelgyro.getMotion6(&ay, &ax, &az, &gx, &gy, &gz);
  
   //make the read values manageable by dividing them a bit
   ax/=200;
   ay/=200;
   az/=200;

   xlog[index]=ax;
   ylog[index]=ay;
   zlog[index]=az;

   fix(&xlog[index]); // limit 0-9
   fix(&ylog[index]);
   fix(&zlog[index]);
  
   index++;
   
   if(index>126) {
     memcpy(xlog,&xlog[1],126);
     memcpy(ylog,&ylog[1],126);
     memcpy(zlog,&zlog[1],126);
     index=126;
   }
   
   //update display 
   display.clear();
   showdata();
   display.update();
}
