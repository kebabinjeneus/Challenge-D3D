/*
*Author: Appie Strorman
*The Challenge
*ESP BaroMeter
*11/12/2019
*V1.0
*/

// Libraries to be able to connect the esp8266 to a WiFi network and to let the LCD and LED work properly.
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// Need to add Display(LCD) and LED
//#define LCD D6
//#define LED D7

// Variables to connect esp to a network
// This is my phone network so I can use it anywhere
const char* ssid = "spot";                // YourWiFiName
const char* password = "?Spot!1234";      // YourWiFiPassword

// Server settings
const char* host = "";

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 4);

void setup() {
  // Open serial connection, to show the result of the program and connect to the WiFi network
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  // Initialize the LCD
  lcd.begin();
  lcd.backlight();
  lcd.print("TEST SCREEN"); 

  // Setup wifi
  Serial.println();
  Serial.print("conecting to: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  delay(1000);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) { // If the ESP isn't connected to a wifi network, display this
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.printf("Connected to the WiFi network: %s\n", ssid);
}

void loop() {
}
