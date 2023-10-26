/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "IoTClassroom_CNM.h"
#include "Colors.h"
#include "Adafruit_SSD1306.h"
#include <neopixel.h>
#include <Encoder.h>


SYSTEM_MODE(SEMI_AUTOMATIC);
SYSTEM_THREAD(ENABLED);
const int PIXELCOUNT = 63;
const int PIXELDELAY =115;
const int startDelay = 1000;
const int OLED_RESET = -1;
Adafruit_NeoPixel pixel(PIXELCOUNT,SPI1,WS2812B);
Adafruit_NeoPixel ring(12,SPI,WS2812B);
Button greenButton(D3);
Button blueButton(D4);
Button encButton(A0);
const int encGreen = D18;
const int encRed = D16;
Adafruit_SSD1306 displayL(OLED_RESET);
Adafruit_SSD1306 displayR(OLED_RESET);
void pixelRace();
void pixelManualRace();
void pixelFill(int pixColor);
void ringFill(int startPixel, int endPixel, int colorPixel);
float startTime, startLapTime1, startLapTime2, lapTime, currentTime, lastTime, finalTime1, finalTime2;
bool autoManual;
bool pitstop;





void setup() {
  displayL.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  displayR.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  displayL.display();
  displayR.display();
  delay(2000); 
  displayL.clearDisplay();
  displayR.clearDisplay();
  displayL.setTextSize(1);
  displayR.setTextSize(1);
  displayL.setTextColor(WHITE);
  displayR.setTextColor(WHITE);
  Serial.begin(9600);
  ring.begin();
  pixel.begin();
  ring.setBrightness(30);
  pixel.setBrightness(40);
  ring.show();
  pixel.show();
  pinMode(encGreen,OUTPUT);
  pinMode(encRed,OUTPUT);
  digitalWrite(encGreen,LOW);
  digitalWrite(encRed,LOW);
  autoManual = true;
}


void loop() {

  if(encButton.isClicked()){
    autoManual = !autoManual;
  }
  while(autoManual == true){
    digitalWrite(encGreen,LOW);
    digitalWrite(encRed,HIGH);
    if(greenButton.isClicked()){
      ringFill(0,3,red);
      delay(startDelay);
      ringFill(0,7,red);
      delay(startDelay);
      ringFill(0,11,red);
      delay(startDelay);
      ringFill(0,11,green);
      startLapTime1 = millis();
      startLapTime2 = millis();
      startTime = millis();
      pixelRace();
      Serial.printf("Race Start\n");
      autoManual =false;
  }
  if(encButton.isClicked()){
    autoManual =  !autoManual;
  }
  }
  while(autoManual == false){
    digitalWrite(encRed,LOW);
    digitalWrite(encGreen,HIGH);
    if(greenButton.isClicked()){
      ringFill(0,3,red);
      delay(startDelay);
      ringFill(0,7,red);
      delay(startDelay);
      ringFill(0,11,red);
      delay(startDelay);
      ringFill(0,11,green);
      startTime = millis();
      pixelManualRace();
    }
    Serial.printf("Manual Mode On\n");
    if(encButton.isClicked()){
    autoManual = !autoManual;
    }
  }
}

 void pixelRace() {
  int color1, color2;
  float speed1, speed2, j,i;
  int k;
  int m =1;
  int n =1;
  color1 = random(0x000000,0xFFFFFF);
  color2 = random(0x000000,0xFFFFFF);
  speed1 = random(75,125)/100.0;
  speed2 = random(75,125)/100.0;
  j=0;
  k=0;
  i=0;
  while(i<310 || j<310) {
    k++;
    pixel.clear();
    pixel.setPixelColor((int)i%62,red);
    pixel.setPixelColor((int)j%62,blue);
    pixel.show();
    currentTime = millis();
    if(currentTime-lastTime>115){
      i = i+speed1;
      j = j+speed2;
      lastTime = currentTime;
    }
    if (k%18==0){
      speed1 = random(75,125)/100.0;
      speed2 = random(75,125)/100.0;
    }
    if(k==20){
      ring.clear();
      ring.show();
    }
    
    if((int)i%62 == 0){
      displayL.clearDisplay();
      displayL.display();
      currentTime = millis();
      lapTime = (currentTime-startLapTime1)/1000.0;
      if (m==5){
        displayL.clearDisplay();
        displayL.display();
        currentTime = millis();
        finalTime1 = (currentTime - startTime)/1000.0;
        displayL.setCursor(0,30);
        displayL.printf("Final %0.3f",finalTime1);
        displayL.display();
      }
      if(lapTime > 3 && m <=4){
        displayL.setCursor(0,30);
        displayL.printf("Lap %i: %0.3f",m,lapTime);
        displayL.display();
        // Serial.printf("Lap %i : %f\n",lapTime);
        startLapTime1 = millis();
        m++;
      }  
    }
    if((int)j%62 == 0){
      displayR.clearDisplay();
      displayR.display();
      currentTime = millis();
      lapTime = (currentTime-startLapTime2)/1000.0;
      if (n==5){
        displayR.clearDisplay();
        displayR.display();
        currentTime = millis();
        finalTime2 = (currentTime - startTime)/1000.0;
        displayR.setCursor(0,30);
        displayR.printf("Final %0.3f",finalTime2);
        displayR.display();
      }
      if(lapTime > 3 && n <=4){
        displayR.setCursor(0,30);
        displayR.printf("Lap %i: %0.3f",n,lapTime);
        displayR.display();
       // Serial.printf("Lap %i : %f\n",lapTime);
        startLapTime2 = millis();
        n++;
        }
      }
    //Serial.printf("i = %f, j = %f, k = %i, Speed1 = %f, Speed2 = %f\n",i,j,k,speed1,speed2);
  }

    if(j > i) {
      pixelFill(blue);
    }
    else{
      pixelFill(red);
    }

}
void pixelFill(int pixColor) {
  int i;
  for(i=0;i<62;i++) {
    pixel.setPixelColor(i,pixColor);
  }
  pixel.show();
}
void ringFill(int startPixel, int endPixel, int colorPixel){
  int i;
  for(i=startPixel; i <= endPixel; i++){
    ring.setPixelColor(i,colorPixel);
    ring.show();   
  }
    return;
}

 void pixelManualRace() {
  int color1, color2;
  int j,i;
  int k;
  color1 = random(0x000000,0xFFFFFF);
  color2 = random(0x000000,0xFFFFFF);
  j=0;
  k=0;
  i=0;
  while(i<124 || j<124) {
    k++;
    pixel.clear();
    pixel.setPixelColor((int)i%62,red);
    pixel.setPixelColor((int)j%62,blue);
    pixel.show();
    currentTime = millis();
    if(greenButton.isClicked()){
      j++;
    }
    if(blueButton.isClicked()){
      i++;
    }
    if(k==5){
      ring.clear();
      ring.show();
    }
    
    if((int)i == 124){
      displayL.clearDisplay();
      displayL.display();
      currentTime = millis();
        finalTime1 = (currentTime - startTime)/1000.0;
        displayL.setCursor(0,30);
        displayL.printf("Final %0.3f",finalTime1);
        displayL.display();
      }
       
    if((int)j == 124){
      displayR.clearDisplay();
      displayR.display();
      currentTime = millis();
        finalTime2 = (currentTime - startTime)/1000.0;
        displayR.setCursor(0,30);
        displayR.printf("Final %0.3f",finalTime2);
        displayR.display();
      }
      }
    //Serial.printf("i = %f, j = %f, k = %i, Speed1 = %f, Speed2 = %f\n",i,j,k,speed1,speed2);
    if(j > i) {
      pixelFill(blue);
    }
    else{
      pixelFill(red);
    }
    }