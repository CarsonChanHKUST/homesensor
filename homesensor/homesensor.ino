#include "Adafruit_CCS811.h"
#include "SparkFun_SCD4x_Arduino_Library.h"
#include <TFT_eSPI.h>
#include <Wire.h>

TFT_eSPI tft = TFT_eSPI();  
TFT_eSprite sprite= TFT_eSprite(&tft);

#define DARKGREEN 0x1CC5 //darkgreen
#define DARKYELLOW 0xBE00 //darkyellow
int CO2Color= DARKGREEN, TVOCColor= DARKGREEN;


// Create the sensor object
Adafruit_CCS811 ccs;
SCD4x mySensor;

int sda = 18;
int scl = 17;

String co2data, tvocdata;
int co2int=0, temperature=0, humidity=0;

int CO2Arr[110]= { };

void setup() {
  
  tft.init();
  tft.setRotation(3);
  sprite.createSprite(320,170);
  Wire.begin(sda, scl);
  Serial.begin(9600);


  if (mySensor.begin() == false)
  {
    Serial.println(F("Sensor not detected. Please check wiring. Freezing..."));
    while (1)
      ;
  }

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
  sprite.drawString("%RH", 65, 125);
  sprite.drawString("TEMP", 200, 125);

  sprite.setTextSize(1);
  sprite.setTextColor(TFT_WHITE, CO2Color);
  sprite.drawString("ppm", 300, 45);
  sprite.drawString("1800", 220, 25);
  sprite.drawString("1000", 220, 45);
  sprite.drawString("600", 220, 65);
  sprite.setTextColor(TFT_WHITE, TVOCColor);
  sprite.drawString("oC", 300, 125);
  sprite.drawString("%", 140, 125);

  // sprite.drawRect(200,20,2,50,TFT_WHITE);//vert
  // sprite.drawRect(90,70,110,2,TFT_WHITE);//horz
  // sprite.drawRect(200,100,2,50,TFT_WHITE);
  // sprite.drawRect(90,150,110,2,TFT_WHITE);
  

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

  


  if (co2int > 1600){
    CO2Color = TFT_RED;
    }else if(co2int > 800){
      CO2Color = DARKYELLOW;
    }else{
      CO2Color = DARKGREEN;
    }
  
  if (temperature > 600){
    TVOCColor = TFT_RED;
  }else if(temperature > 300){
    TVOCColor = DARKYELLOW;
  }else{
    TVOCColor = DARKGREEN;
  }
  drawGraph();
  
  sprite.setTextSize(2);
  sprite.setTextColor(TFT_WHITE, CO2Color);
  sprite.drawString(String(co2int), 260, 45);
  sprite.setTextColor(TFT_WHITE, TVOCColor);
  sprite.drawString(String(temperature), 270, 125);
  sprite.drawString(String(humidity), 120, 125);

  for(int i=110;i>1;i--)
  {
      if (CO2Arr[i]>40){
      sprite.drawPixel(90+i,70-CO2Arr[i],TFT_RED);
      }else if(CO2Arr[i]>20){
      sprite.drawPixel(90+i,70-CO2Arr[i],TFT_YELLOW);
      }else{
      sprite.drawPixel(90+i,70-CO2Arr[i],TFT_GREEN);
      }
  }
  

  sprite.pushSprite(0,0);
  
  
  // sprite.setTextColor(TFT_WHITE, TEMPColor);
  // sprite.drawString(String(temp), 265, 85);
}

void loop() {

    
    if (mySensor.readMeasurement()) // readMeasurement will return true when fresh data is available
  {
    Serial.println();

    Serial.print(F("CO2(ppm):"));
    co2int = mySensor.getCO2();
    Serial.print(co2int);
    CO2Arr[109]=((co2int/32)-12);

    Serial.print(F("\tTemperature(C):"));
    temperature = mySensor.getTemperature();
    Serial.print(temperature, 1);

    Serial.print(F("\tHumidity(%RH):"));
    humidity = mySensor.getHumidity();
    Serial.print(humidity, 1);

    Serial.println();
    update_data();
  }
  else
    Serial.print(F("."));

  delay(500);


}
