/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include <neopixel.h>
#include "Colors.h"

SYSTEM_MODE(SEMI_AUTOMATIC);
const int PIXELCOUNT = 63;
const int PIXELDELAY =160;
Adafruit_NeoPixel pixel(PIXELCOUNT,SPI1,WS2812B);
void pixelRace();
void pixelFill(int pixColor);

SYSTEM_THREAD(ENABLED);





void setup() {
  Serial.begin(9600);
  pixel.begin();
  pixel.setBrightness(40);
  pixel.show();
}


void loop() {
  pixelRace();
}


 void pixelRace() {
  int color1, color2;
  float speed1, speed2, j,i;
  int k;
  color1 = random(0x000000,0xFFFFFF);
  color2 = random(0x000000,0xFFFFFF);
  speed1 = random(80,120)/100.0;
  speed2 = random(80,120)/100.0;
  j=0;
  k=0;
  i=0;
  while(i<186 && j<186) {
    k++;
    pixel.clear();
    pixel.setPixelColor((int)i%62,red);
    pixel.setPixelColor((int)j%62,blue);
    pixel.show();
    j = (j + speed2);
    i = (i + speed1);
    if (k%10==0){
      speed1 = random(80,120)/100.0;
      speed2 = random(80,120)/100.0;
    }
    Serial.printf("i = %f, j = %f, k = %i, Speed1 = %f, Speed2 = %f\n",i,j,k,speed1,speed2);
    delay(PIXELDELAY);
  }

    if(j > i) {
      pixelFill(blue);
    }
    else{
      pixelFill(red);
    }
    delay(5000);
}
void pixelFill(int pixColor) {
  int i;
  for(i=0;i<62;i++) {
    pixel.setPixelColor(i,pixColor);
  }
  pixel.show();
}