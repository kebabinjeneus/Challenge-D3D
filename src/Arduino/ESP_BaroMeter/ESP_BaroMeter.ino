/*
*Author: Appie Strorman
*The Challenge
*ESP BaroMeter
*18/12/2019
*V1.1
*TOOLS/BOARD/"NodeMCU 1.0 (ESP-12E Module)"
*Additional Boards Manager URL (Paste): http://arduino.esp8266.com/stable/package_esp8266com_index.json
*Libraries:
*https://github.com/marcoschwartz/LiquidCrystal_I2C/archive/master.zip
*https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi
*/

// Libraries to be able to connect the esp8266 to a WiFi network and to let the LCD and LED work properly.
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>

// Need to add Display(LCD) and LED
//#define LCD D6
//#define LED D7

/*WIFI SETTINGS*/
// Variables to connect esp to a network
// This is my phone network so I can use it anywhere
// IDEA, connect esp with the raspberry pi, that will give the SSID and PASSWORD to the ESP8266 for easy connection
const char* ssid = "spot";            // YourWiFiName
const char* password = "?Spot!1234";  // YourWiFiPassword

// Server settings
const char *host = "192.168.56.1";    //Your pc or server (database) IP, example : 192.168.0.0 , if you are a windows os user, open cmd, then type ipconfig then look at IPv4 Address.

/*LCD SETTINGS*/
// set the LCD number of columns and rows
int LCDColumns = 16;
int LCDRows = 2;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, LCDColumns, LCDRows);

/*Hardcoded text to be displayed at the LCD screen, this info must be sent 
  from the Raspberry Pi, but a connection must be established first*/
float used_Today_kWh = 28.9f;
float used_ThisHour_kWh = 0.02069f;

void setup() {
  // Open serial connection, to show the result of the program and connect to the WiFi network
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot

  // Initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

  // Connect to the WIFI
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
  
  //If connection is successful, show IP address in serial monitor
  Serial.println("");
  Serial.printf("Connected to Network", ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  // Show on LCD
  lcd.setCursor(0,0);
  lcd.printf("Connected to Network");
  delay(2000);
  lcd.clear();
}

void loop() {
  /*Display of how much energy has been used today*/
  lcd.setCursor(0,0); // set cursor to first column, first row
  Serial.printf("Today: %.6f*kWh\n", used_Today_kWh);
  lcd.printf("Today: %.6f", used_Today_kWh);
  lcd.setCursor(0,1); // set cursor to first column, second row
  lcd.print("*kWh");
  delay(5000);        // Refresh screen every 5 seconds
  lcd.clear();        // clears the display to print new message
  lcd.setCursor(0,0); // set cursor to first column, second row
  Serial.printf("Last hour: %.6f *kWh\n", used_ThisHour_kWh);
  lcd.printf("Last hour: %.6f", used_ThisHour_kWh);
  lcd.setCursor(0,1); // set cursor to first column, second row
  lcd.print("*kWh");
  delay(5000);        // Refresh screen every 5 seconds
  lcd.clear();        // clears the display to print new message

//float max_used_ThisHour_kWh = 0.10000f;
//if (max_used_ThisHour > used_ThisHour) {
//  // LED Strip must turn Red
//
//} else {
//  // LED Strip must turn Green
//
//}
}
