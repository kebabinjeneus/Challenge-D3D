/*
*Author: Appie Strorman
*The Challenge
*ESP BaroMeter
*11/12/2019
*V1.0
*TOOLS/BOARD/"NodeMCU 1.0 (ESP-12E Module)"
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
const char* ssid = "spot";            // YourWiFiName
const char* password = "?Spot!1234";  // YourWiFiPassword

// Server settings
const char *host = "192.168.56.1";    //Your pc or server (database) IP, example : 192.168.0.0 , if you are a windows os user, open cmd, then type ipconfig then look at IPv4 Address.

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 4);

void setup() {
  // Open serial connection, to show the result of the program and connect to the WiFi network
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);
  Serial.println("");

   // Setup wifi
  Serial.println();
  Serial.print("Connecting...");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.println("Connecting to WiFi...");
    delay(250);
  }
  
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.printf("Connected to Network", ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  // Initialize the LCD
  lcd.begin();
  lcd.backlight();
  lcd.print("TEST SCREEN"); 
}

void loop() {

  
}
