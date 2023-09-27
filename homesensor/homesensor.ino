#include "Adafruit_CCS811.h"
#include <TFT_eSPI.h>
#include <Wire.h>

TFT_eSPI tft = TFT_eSPI();  
TFT_eSprite sprite= TFT_eSprite(&tft);

#define DARKGREEN 0x1CC5 //darkgreen
#define DARKYELLOW 0xBE00 //darkyellow
int CO2Color= DARKGREEN, TVOCColor= DARKGREEN;


// Create the sensor object
Adafruit_CCS811 ccs;

int sda = 18;
int scl = 17;

String co2data, tvocdata;
int co2int=0, tvocint=0;

int CO2Arr[110]= { };
int TVOCArr[110]= { };

void setup() {
  
  tft.init();
  tft.setRotation(3);
  sprite.createSprite(320,170);
  Wire.begin(sda, scl);
  Serial.begin(9600);

  Serial.println("CCS811 test");

  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
  }

  // Wait for the sensor to be ready
  while(!ccs.available());
  drawGraph();
}


void drawGraph() //background elements
  { 
  sprite.fillSprite(TFT_BLACK);
  sprite.fillRoundRect(20,10,300,70,10,CO2Color);  //CO2
  sprite.fillRoundRect(20,90,300,70,10,TVOCColor); //TVOC

  sprite.setTextDatum(4);
  sprite.setTextSize(3);
  sprite.setTextColor(TFT_WHITE, CO2Color);
  sprite.drawString("CO2", 55, 45);
  sprite.setTextColor(TFT_WHITE, TVOCColor);
  sprite.drawString("TVOC", 65, 125);

  sprite.setTextSize(1);
  sprite.setTextColor(TFT_WHITE, CO2Color);
  sprite.drawString("ppm", 300, 45);
  sprite.drawString("1800", 220, 25);
  sprite.drawString("1000", 220, 45);
  sprite.drawString("600", 220, 65);
  sprite.setTextColor(TFT_WHITE, TVOCColor);
  sprite.drawString("ppb", 300, 125);
  sprite.drawString("600", 220, 105);
  sprite.drawString("300", 220, 125);
  sprite.drawString("0", 220, 145);

  // sprite.drawRect(200,20,2,50,TFT_WHITE);//vert
  // sprite.drawRect(90,70,110,2,TFT_WHITE);//horz
  // sprite.drawRect(200,100,2,50,TFT_WHITE);
  // sprite.drawRect(90,150,110,2,TFT_WHITE);
  
//future development: use sprite.drawPixel(60+j,110-(i*10),color1); to draw the linegraph how the trend goes.

  //sprite.pushSprite(0,0);
  }

void update_data(){
  
  //if have time optimize to no operation when zero is in the array
  //shifting array element to the left

  int temp = CO2Arr[0]; //remember first element
  for(int i=0;i<110-1;i++)
  {
      CO2Arr[i] = CO2Arr[i+1]; //move all element to the left except first one
  }
  CO2Arr[110-1] = temp;
  temp = TVOCArr[0]; //remember first element
  for(int i=0;i<110-1;i++)
  {
      TVOCArr[i] = TVOCArr[i+1]; //move all element to the left except first one
  }
  TVOCArr[110-1] = temp;
  


  if (co2int > 1600){
    CO2Color = TFT_RED;
    }else if(co2int > 800){
      CO2Color = DARKYELLOW;
    }else{
      CO2Color = DARKGREEN;
    }
  
  if (tvocint > 600){
    TVOCColor = TFT_RED;
  }else if(tvocint > 300){
    TVOCColor = DARKYELLOW;
  }else{
    TVOCColor = DARKGREEN;
  }
  drawGraph();
  
  sprite.setTextSize(2);
  sprite.setTextColor(TFT_WHITE, CO2Color);
  sprite.drawString(String(co2int), 260, 45);
  sprite.setTextColor(TFT_WHITE, TVOCColor);
  sprite.drawString(String(tvocint), 270, 125);

  for(int i=110;i>1;i--)
  {
      if (CO2Arr[i]>40){
      sprite.drawPixel(90+i,70-CO2Arr[i],TFT_RED);
      }else if(CO2Arr[i]>20){
      sprite.drawPixel(90+i,70-CO2Arr[i],TFT_YELLOW);
      }else{
      sprite.drawPixel(90+i,70-CO2Arr[i],TFT_GREEN);
      }
      if (TVOCArr[i]>40){
      sprite.drawPixel(90+i,150-TVOCArr[i],TFT_RED);
      }else if(TVOCArr[i]>20){
      sprite.drawPixel(90+i,150-TVOCArr[i],TFT_YELLOW);
      }else{
      sprite.drawPixel(90+i,150-TVOCArr[i],TFT_GREEN);
      }

  }
  

  sprite.pushSprite(0,0);
  
  
  // sprite.setTextColor(TFT_WHITE, TEMPColor);
  // sprite.drawString(String(temp), 265, 85);
}

void loop() {
  
  if(ccs.available()){
      if(!ccs.readData()){
        Serial.print("CO2: ");
        co2int = ccs.geteCO2();
        CO2Arr[109]=((co2int/32)-12);
        Serial.print(co2int/32-12);
        Serial.print("ppm, TVOC: ");
        tvocint = ccs.getTVOC();
        TVOCArr[109]=tvocint/10;
        Serial.println(tvocint);
        update_data();
      }
      else{
        Serial.println("ERROR!");
        while(1);
      }
    }
    delay(500);


}
