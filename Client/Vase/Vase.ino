/******************************************************
 ******************************************************
  ____       _           _     ____  _             _   
 |  _ \ ___ | |__   ___ | |_  |  _ \| | __ _ _ __ | |_ 
 | |_) / _ \| '_ \ / _ \| __| | |_) | |/ _` | '_ \| __|
 |  _ < (_) | |_) | (_) | |_  |  __/| | (_| | | | | |_ 
 |_| \_\___/|_.__/ \___/ \__| |_|   |_|\__,_|_| |_|\__|
 ******************************************************                                                  
 ******************************************************
 **Created 27 Jan 2018*********************************
 **By Guilherme Uezima*********************************
 ******************************************************
 ======================================================
 *      Monitoring of sensors by Arduino Uno and      *
 *           communication with the server            *
 ======================================================*/

#include <DHT.h>                //Temperature and air pression sensor lib
#include <Adafruit_NeoPixel.h>  //RGB LED Circle lib
#include "LedControl.h"         //8x8 LED Matrix lib
#include <Wire.h>               //Clock lib
#include "RTClib.h"             //RTC Clock Module lib

/* DEFINE BLOC ======= */
#define LIGHT_SENSOR_PIN A0
#define TEMP_PRESSION_PIN A1
#define SOIL_HUMIDITY_PIN A2
#define DHTTYPE DHT11

/* CONST BLOCK ======= */
const int LED_DIN_PIN = 12;
const int LED_CS_PIN = 10;
const int LED_CLK_PIN = 11;
const int LED_RGB_CIRCLE_PIN = 9;
const int WATER_BOMB_PIN = 8;
const int MAIN_LIGHT_PIN = 7;

/* VARS BLOCK ======= */
int turnOnMainLightHours[] = {12, 13, 16, 20, 21, 22, 23, 0, 1, 2, 3, 4};
int turnOnLedCircleHours[] = {14, 15, 17, 18, 19, 5, 6, 7, 8, 9, 10, 11};
int turnOnWaterBombHours[] = {19};

int sizeArrayTurnOnMainLightHours = sizeof(turnOnMainLightHours) / sizeof(turnOnMainLightHours[0]);
int sizeArrayTurnOnLedCircleHours = sizeof(turnOnLedCircleHours) / sizeof(turnOnLedCircleHours[0]);
int sizeArrayTurnOnWaterBombHours = sizeof(turnOnWaterBombHours) / sizeof(turnOnWaterBombHours[0]);

int currentTime = -1;
int currentTimeRTC = -1;
int lightSensor = 0;
int soilHumiditySensor = 0;
int countChangeColor = 0;
float airHumidity = 0;
float temperature = 0;
bool changeColor = true;
int mainLightStatus = 0;
int circleLedStatus = 0;
int waterBombStatus = 0;

/* TEMP/PRESSURE SENSOR ======= */
DHT dht(TEMP_PRESSION_PIN, DHTTYPE);

void readAirHumiditySensor(){
  airHumidity = dht.readHumidity();
}

void readTemperatureSensor(){
  temperature = dht.readTemperature();
}

/* LIGHT INTENSITY SENSOR ======= */
void readLightIntensitySensor(){
  lightSensor = analogRead(LIGHT_SENSOR_PIN);
}

/* SOIL HUMIDITY SENSOR ======= */
void readSoilSensor(){
  soilHumiditySensor = analogRead(SOIL_HUMIDITY_PIN);
}

/* 8X8 LED MATRIX ======= */
LedControl ledControl = LedControl(LED_DIN_PIN, LED_CLK_PIN, LED_CS_PIN, 1);
unsigned long delaytime = 250;

void happyFaceLEDMatrix() {
  ledControl.clearDisplay(0);
  ledControl.setRow(0, 0, 0x66);
  ledControl.setRow(0, 1, 0x99);
  ledControl.setRow(0, 2, 0x99);
  ledControl.setRow(0, 3, 0x66);
  ledControl.setRow(0, 4, 0x00);
  ledControl.setRow(0, 5, 0x81);
  ledControl.setRow(0, 6, 0x42);
  ledControl.setRow(0, 7, 0x3C);
  delay(1000);
}

void sadFaceLEDMatrix() {
  ledControl.clearDisplay(0);
  ledControl.setRow(0, 0, 0x66);
  ledControl.setRow(0, 1, 0x99);
  ledControl.setRow(0, 2, 0x99);
  ledControl.setRow(0, 3, 0x66);
  ledControl.setRow(0, 4, 0x00);
  ledControl.setRow(0, 5, 0x3C);
  ledControl.setRow(0, 6, 0x42);
  ledControl.setRow(0, 7, 0x81);
  delay(1000);
}

/* RGB CIRCLE LED ======= */
Adafruit_NeoPixel circleRGBLed = Adafruit_NeoPixel(16, LED_RGB_CIRCLE_PIN, NEO_GRB + NEO_KHZ800);

void changeCircleLEDColor(uint16_t red, uint16_t green, uint16_t blue) {
  for (uint16_t i = 0; i < circleRGBLed.numPixels(); i++) {
    circleRGBLed.setPixelColor(i, red, green, blue);
    circleRGBLed.show();
  }
}

void setCircleLEDStatus(bool newStatus){
  if(newStatus){
    changeCircleLEDColor(random(0, 255), random(0, 255), random(0, 255));
    circleLedStatus = 1;   
  }else{
    changeCircleLEDColor(0, 0, 0);
    circleLedStatus = 0;
  }
}

/* CLOCK ======= */
RTC_DS1307 RTC;

void readCurrentTime(){
  DateTime now = RTC.now();
  currentTimeRTC = now.hour();
}

void checkTime(){
  if(currentTime != currentTimeRTC){
    
    //verification if its time to turn on main light
    if(arrayIncludeElement(turnOnMainLightHours, currentTimeRTC, sizeArrayTurnOnMainLightHours)){
      setMainLightStatus(true);
      setCircleLEDStatus(false);
    }

    //verification if its time to turn on circle LED
    if(arrayIncludeElement(turnOnLedCircleHours, currentTimeRTC, sizeArrayTurnOnLedCircleHours)){
      setMainLightStatus(false);
      setCircleLEDStatus(true);
    }

    //verification if its time to turn on water bomb
    if(arrayIncludeElement(turnOnWaterBombHours, currentTimeRTC, sizeArrayTurnOnWaterBombHours)){
      turnOnWaterBomb();
    }

    currentTime = currentTimeRTC;
  }
}

/* WATER BOMB BLOCK ========= */
void turnOnWaterBomb(){
  digitalWrite(WATER_BOMB_PIN, HIGH);
  delay(1000);
  digitalWrite(WATER_BOMB_PIN, LOW);
}

/* MAIN LIGHT BLOCK ========= */
void setMainLightStatus(bool newStatus){
  if(newStatus){
    digitalWrite(MAIN_LIGHT_PIN, LOW);
    mainLightStatus = 1;
  }else{
    digitalWrite(MAIN_LIGHT_PIN, HIGH);
    mainLightStatus = 0;
  }
}


/* AUXILIAR BLOCK ========= */
void infoLog(String msg) {
  Serial.println("[INFO] " + msg);
}

boolean arrayIncludeElement(int hourArray[], int element, int sizeArray) {
  for (int i = 0; i < sizeArray; i++) {
    if (hourArray[i] == element) {
      return true;
    }
  }
  return false;
}

/* SETUP BLOCK ======= */
void setup() {
  Serial.begin(115200);
  
  dht.begin();
  
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  pinMode(SOIL_HUMIDITY_PIN, INPUT);
  pinMode(MAIN_LIGHT_PIN, OUTPUT);
  pinMode(WATER_BOMB_PIN, OUTPUT);

  circleRGBLed.begin();
  circleRGBLed.setBrightness(30);

  ledControl.shutdown(0, false);
  ledControl.setIntensity(0, 10);
  ledControl.clearDisplay(0);

  changeCircleLEDColor(random(0, 255), random(0, 255), random(0, 255));

  Wire.begin();
  RTC.begin();
  RTC.adjust(DateTime(__DATE__,__TIME__));
}

void loop() {
  readCurrentTime();
  readSoilSensor();
  readAirHumiditySensor();
  readTemperatureSensor();
  readLightIntensitySensor();

  checkTime();

  happyFaceLEDMatrix();
}
