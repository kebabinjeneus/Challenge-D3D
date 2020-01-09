/*
 * Author: Appie Strorman
 * The Challenge
 * ESP BaroMeter
 * 18/12/2019
 * V1.3
 * Includes:
 * LCD connection
 * Led Strip connection
 * If the WiFi connection fails, then the leds turn white and the display displays Reconnecting to WiFi...
 * 
 * Missing:
 * Connection with the raspberry Pi
 * Algortihm to display the correct led color
 * 
 * TOOLS/BOARD/"Adaruit Feather HUZZAH ESP8266"
 * Additional Boards Manager URL (Paste): http://arduino.esp8266.com/stable/package_esp8266com_index.json
 * 
 * Libraries:
 * https://github.com/marcoschwartz/LiquidCrystal_I2C/archive/master.zip
 * https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi
 *
 ******************
 *Connection LCD: *
 ******************
 * LCD -> ESP     *
 ******************
 * GND -> GND     *
 * VCC -> VIN     *
 * SDA -> D2      *
 * SCL -> D1      *
 ******************
 *
 * Documentation:
 * https://github.com/FastLED/FastLED/wiki/Basic-usage
 * 
 * Color-picker:
 * https://www.w3schools.com/colors/colors_picker.asp
*/

// Libraries to be able to connect the esp8266 to a WiFi network and to let the LCD and LED work properly.
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include "FastLED.h"

// Led Strip macro definitions
#define NUM_LEDS 8                              // Number of leds: 8
#define DATA_PIN 14                             // Connected to GPIO 14, NodeMCU pin D5
#define BRIGHTNESS 120                          // Sets the brightness of the leds between 0-255 

// WiFi Setup
// Variables to connect esp to a network
// This is my phone/laptop hotspot network so I can use it anywhere
// IDEA, connect esp with the raspberry pi, that will give the SSID and PASSWORD to the ESP8266 for easy connection
const char* ssid="spot";                        // YourWiFiName
const char* password="?Spot!1234";              // YourWiFiPassword
const char* host="192.168.56.1";                // LOCAL IPv4 ADDRESS...ON CMD WINDOW TYPE ipconfig/all

/*LCD SETTINGS*/
// set the LCD number of columns and rows
int LCDColumns = 20;
int LCDRows = 4;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, LCDColumns, LCDRows);

// Block of memory that will be used for storing and manipulating the led data
// This sets up an array that we can manipulate to set/clear led data
CRGB leds[NUM_LEDS];

/*Hardcoded text to be displayed at the LCD screen, this info must be sent 
  from the Raspberry Pi, but a connection must be established first*/
float used_today_kWh = 8.4f;
float used_current_kWh = 0.0154f;
unsigned short int used_today_gas_m3 = 500; // Not negative and from 0 to 65,535, kubieke meter

void setup(){
  // Open serial connection, to show the result of the program and connect to the WiFi network
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);                            //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);                            //This line hides the viewing of ESP as wifi hotspot

  // LCD setup
  lcd.init();                                     // Initialize LCD
  lcd.backlight();                                // turn on LCD backlight

  // Connection of the led, tells the library that the led is connected on pin 14 and how many leds there are
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);              // The led strip must stay off until there is a WiFi connection
  all_leds_white();                               // Call function all_leds_white to make the led strip white
  
  // Connect to the WIFI
  WiFi.begin(ssid, password);
  
  // Wait for connection
  while(WiFi.status() != WL_CONNECTED){
    delay(250);
    Serial.println("Connecting to WiFi...");
    delay(250);
  }
  
  //If connection is successful, show IP address in serial monitor
  Serial.printf("Connected to Network", ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());                 //IP address assigned to your ESP
  
  // Show on LCD
  lcd.setCursor(0,0);
  lcd.printf("Connected to Network");
  delay(2000);
  lcd.clear();
}

void loop(){
  led_algortihm();
    
  //Display of how much current is used at the moment and has been used today
  Serial.println("      Bewust-E");
  Serial.println("Stroomverbruik:");
  Serial.printf("Huidig %.4f kWh\n", used_current_kWh);
  Serial.printf("Dag:   %.4f kWh\n", used_today_kWh);
  // LCD Scherm
  lcd.setCursor(6,0);                             // Set cursor to third column, first row
  lcd.printf("Bewust-E");
  lcd.setCursor(0,1);                             // Set cursor to first column, second row
  lcd.print("Stroomverbruik:");
  lcd.setCursor(0,2);
  lcd.printf("Huidig:   %.4f kWh", used_current_kWh);
  lcd.setCursor(0,3);
  lcd.printf("Dag:      %.4f kWh", used_today_kWh);
  delay(7000);                                    // Refresh screen every 7 seconds
  lcd.clear();                                    // Clears the display to print new message

  //Display of how much gas has been used today
  Serial.println("      Bewust-E");
  Serial.println("Gasverbruik:");
  Serial.printf("Dag: %hu m3\n", used_today_gas_m3);
  
  // LCD Screen
  lcd.setCursor(6,0);                             // Set cursor to third column, first row
  lcd.printf("Bewust-E");
  lcd.setCursor(0,1);                             // Set cursor to first column, second row
  lcd.print("Gasverbruik:");
  lcd.setCursor(0,2);
  lcd.printf("Dag:          %hu m3", used_today_gas_m3);
  delay(7000);                                    // Refresh screen every 7 seconds
  lcd.clear();                                    // Clears the display to print new 

/*
LCD screen display layout:
    Bewust-E
Stroomverbruik:
Huidig:
Dag:

5 sec delay

    Bewust-E
Gasverbruik:
Dag:
*/

  // Wait for connection
  while(WiFi.status() != WL_CONNECTED){
    all_leds_white();                             // Call function all_leds_white to make the led strip white to indicate that the connection has failed
    Serial.println("No connection");
    lcd.clear();
    lcd.setCursor(0,0);                           // Set cursor to third column, first row
    lcd.printf("No connection");
    lcd.setCursor(0,1);                           // Set cursor to first column, second row
    lcd.printf("Please reset device");
    delay(1000);

    // Reconnect to the WiFi
    WiFi.begin(ssid, password);
  }
}

void led_algortihm() {
  // PLACE HOLDER
  //leds[ID] = CRGB(red, green, blue) Only use value of 10 to reduce the amp draw of the strip when lit
  leds[0]=CRGB(0,255,0);                          // GREEN
  leds[1]=CRGB(0,255,0);                          // GREEN
  leds[2]=CRGB(191,255,0);                        // LIGHT-GREEN
  leds[3]=CRGB(255,255,0);                        // YELLOW
  leds[4]=CRGB(255,191,0);                        // ORANGE
  leds[5]=CRGB(255,64,0);                         // LIGHT-RED
  leds[6]=CRGB(255,0,0);                          // RED
  leds[7]=CRGB(255,0,0);                          // RED
  FastLED.show();

//float max_used_ThisHour_kWh = 0.10000f;
//if (max_used_ThisHour > used_ThisHour){
//  // LED Strip must turn Red
//
//} else{
//  // LED Strip must turn Green
//
//}
}

// Function all_leds_white sets all the leds white
void all_leds_white() {
  //leds[ID] = CRGB(red, green, blue) Only use value of 10 to reduce the amp draw of the strip when lit
  leds[0]=CRGB::White;                          // WHITE
  leds[1]=CRGB::White;                          // WHITE
  leds[2]=CRGB::White;                          // WHITE
  leds[3]=CRGB::White;                          // WHITE
  leds[4]=CRGB::White;                          // WHITE
  leds[5]=CRGB::White;                          // WHITE
  leds[6]=CRGB::White;                          // WHITE
  leds[7]=CRGB::White;                          // WHITE
  FastLED.show();
}
