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
  sprite.drawString("ppm", 240, 45);
  sprite.drawString("1800", 190, 25);
  sprite.drawString("1000", 190, 45);
  sprite.drawString("600", 190, 65);
  sprite.setTextColor(TFT_WHITE, TVOCColor);
  sprite.drawString("ppb", 240, 125);
  sprite.drawString("1200", 190, 105);
  sprite.drawString("600", 190, 125);
  sprite.drawString("0", 190, 145);
  
//future development: use sprite.drawPixel(60+j,110-(i*10),color1); to draw the linegraph how the trend goes.

  //sprite.pushSprite(0,0);
  }

void update_data(){
  
  if (co2int > 1600){
    CO2Color = TFT_RED;
    }else if(co2int > 800){
      CO2Color = DARKYELLOW;
    }else{
      CO2Color = DARKGREEN;
    }
  
  if (tvocint > 1200){
    TVOCColor = TFT_RED;
  }else if(tvocint > 600){
    TVOCColor = DARKYELLOW;
  }else{
    TVOCColor = DARKGREEN;
  }
  drawGraph();
  
  sprite.setTextSize(2);
  sprite.setTextColor(TFT_WHITE, CO2Color);
  sprite.drawString(String(co2int), 190, 45);
  sprite.setTextColor(TFT_WHITE, TVOCColor);
  sprite.drawString(String(tvocint), 210, 125);
  sprite.pushSprite(0,0);
  
  
  // sprite.setTextColor(TFT_WHITE, TEMPColor);
  // sprite.drawString(String(temp), 265, 85);
}

void loop() {
  
  if(ccs.available()){
      if(!ccs.readData()){
        Serial.print("CO2: ");
        co2int = ccs.geteCO2();
        Serial.print(co2data);
        Serial.print("ppm, TVOC: ");
        tvocint = ccs.getTVOC();
        Serial.println(tvocdata);
        update_data();
      }
      else{
        Serial.println("ERROR!");
        while(1);
      }
    }
    delay(500);


}
