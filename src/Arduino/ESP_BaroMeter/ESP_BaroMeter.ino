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
const char* host = "192.168.56.1";    //Your pc or server (database) IP, example : 192.168.0.0 , if you are a windows os user, open cmd, then type ipconfig then look at IPv4 Address.

/*LCD SETTINGS*/
// set the LCD number of columns and rows
int LCDColumns = 20;
int LCDRows = 4;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, LCDColumns, LCDRows);

/*Hardcoded text to be displayed at the LCD screen, this info must be sent 
  from the Raspberry Pi, but a connection must be established first*/
float used_today_kWh = 8.4f;
float used_current_kWh = 0.0154f;
unsigned short int used_today_gas_m3 = 500; // Not negative and from 0 to 65,535, kubieke meter

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
  /*Display of how much current is used at the moment and has been used today*/
  Serial.println("      Bewust-E");
  Serial.println("Stroomverbruik:");
  Serial.printf("Huidig %.4f kWh\n", used_current_kWh);
  Serial.printf("Dag: %.4f kWh\n", used_today_kWh);
  // LCD Scherm
  lcd.setCursor(6,0); // Set cursor to third column, first row
  lcd.printf("Bewust-E");
  lcd.setCursor(0,1); // Set cursor to first column, second row
  lcd.print("Stroomverbruik:");
  lcd.setCursor(0,2);
  lcd.printf("Huidig:   %.4f kWh", used_current_kWh);
  lcd.setCursor(0,3);
  lcd.printf("Dag:      %.4f kWh", used_today_kWh);
  delay(7000);        // Refresh screen every 5 seconds
  lcd.clear();        // Clears the display to print new message

  /*Display of how much gas has been used today*/
  Serial.println("      Bewust-E");
  Serial.println("Gasverbruik:");
  Serial.printf("Dag: %hu m3\n", used_today_gas_m3);
  // LCD Scherm
  lcd.setCursor(6,0); // Set cursor to third column, first row
  lcd.printf("Bewust-E");
  lcd.setCursor(0,1); // Set cursor to first column, second row
  lcd.print("Gasverbruik:");
  lcd.setCursor(0,2);
  lcd.printf("Dag:          %hu m3", used_today_gas_m3);
  delay(7000);        // Refresh screen every 5 seconds
  lcd.clear();        // Clears the display to print new 

// LCD screen display layout:
//    Bewust-E
//Stroom:
//Huidig:
//Dag:
//
//5 sec delay
//
//    Bewust-E
//Gas:
//Dag:



//float max_used_ThisHour_kWh = 0.10000f;
//if (max_used_ThisHour > used_ThisHour) {
//  // LED Strip must turn Red
//
//} else {
//  // LED Strip must turn Green
//
//}
}
