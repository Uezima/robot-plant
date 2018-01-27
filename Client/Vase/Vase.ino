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
const int turnOnMainLightHours[] = {12, 13, 15, 20, 21, 22, 23, 0, 1, 2, 3, 4};
const int turnOnLedCircleHours[] = {14, 16, 17, 18, 19, 5, 6, 7, 8, 9, 10, 11};
const int turnOnWaterBombHours[] = {19};
const int LED_DIN_PIN = 12;
const int LED_CS_PIN = 10;
const int LED_CLK_PIN = 11;
const int LED_RGB_CIRCLE_PIN = 9;
const int WATER_BOMB_PIN = 8;
const int MAIN_LIGHT_PIN = 7;

/* VARS BLOCK ======= */
int currentTime = -1;
int lightSensor = 0;
int currentTimeRTC = -1;
int soilHumidityValue = 0;
int countChangeColor = 0;
float airHumidity = 0;
float temperature = 0;
bool changeColor = true;

/* TEMP/PRESSURE SENSOR ======= */
DHT dht(TEMP_PRESSION_PIN, DHTTYPE);

/* 8X8 LED MATRIX ======= */
LedControl ledControl = LedControl(LED_DIN_PIN, LED_CLK_PIN, LED_CS_PIN, 1);
unsigned long delaytime = 250;

/* RGB CIRCLE LED ======= */
Adafruit_NeoPixel circleRGBLed = Adafruit_NeoPixel(16, LED_RGB_CIRCLE_PIN, NEO_GRB + NEO_KHZ800);

/* LOG BLOCK ========= */
void infoLog(String msg) {
  Serial.println("[INFO] " + msg);
}

/* SETUP BLOCK ======= */
void setup() {
  Serial.begin(115200);
}

void loop() {

}
