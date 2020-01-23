/*
 * Author: Appie Strorman
 * The Challenge
 * ESP BaroMeter
 * 1/23/2020
 * V1.7
 * 
 * Includes:
 * LCD connection.
 * Led Strip connection.
 * If the WiFi connection fails, then the leds turn white and the display displays Reconnecting to WiFi...
 * Connection with the MQTT Broker CLOUDMQTT: throught this the esp can communicate with the Raspberry Pi, it will read the content that is published bij the Raspberry Pi.
 * Connection between the Raspberry Pi Zero W and the ESP using the MQTT broker thats running local.
 * Algortihm to display the correct led color.
 * Live presentation (10 secs) of the kWh on the LCD and on the Serial Monitor
 * Sends HTTP request to the Apache2 server.
 * Two buttons, red is the simulation of a washing machine, green is the simulation of a tv.
 * 
 * TOOLS/BOARD/"Adaruit Feather HUZZAH ESP8266"
 * Additional Boards Manager URL (Paste): http://arduino.esp8266.com/stable/package_esp8266com_index.json
 * 
 * Libraries:
 * ArduinoJSOn:         https://github.com/bblanchon/ArduinoJson/archive/v5.13.5.zip
 * ESP8266HTTPClient:   https://codeload.github.com/manrueda/ESP8266HttpClient/zip/master
 * ESP8266WiFi:         https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi
 * FastLED:             https://codeload.github.com/FastLED/FastLED/zip/master
 * Liquid Crystal:      https://github.com/marcoschwartz/LiquidCrystal_I2C/archive/master.zip
 * PubSubClient(MQTT):  https://codeload.github.com/knolleary/pubsubclient/zip/v2.
 *
 * Documentation:
 * https://github.com/FastLED/FastLED/wiki/Basic-usage
 * https://randomnerdtutorials.com/decoding-and-encoding-json-with-arduino-or-esp8266/
 * 
 * Color-picker:
 * https://www.w3schools.com/colors/colors_picker.asp
 * 
 *******************
 * Connection LCD: *
 *******************
 * LCD -> ESP      *
 *******************
 * GND -> GND      *
 * VCC -> VIN      *
 * SDA -> D2       *
 * SCL -> D1       *
 *******************
*/

// Libraries to be able to connect the esp8266 to a WiFi network, let the LCD and LED work properly and allow us to connect to a MQTT broker and publish/subscribe messages in topics
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include "FastLED.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Led Strip macro definitions
#define NUM_LEDS 8                                // Number of leds: 8
#define DATA_PIN 14                               // Connected to GPIO 14, NodeMCU pin D5
#define BRIGHTNESS 120                            // Sets the brightness of the leds between 0-255 

// Buttons definitions
#define buttonGreen 12                            // D6 on esp
#define buttonRed 13                              // D7 on esp
int buttonStateGreen = 0;
int buttonStateRed = 0;

// WiFi setup
// Variables to connect the esp to a network
const char* ssid = "HotspotDaan";                 // YourWiFiName
const char* password = "tireda1234";              // YourWiFiPassword

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "192.168.137.1";
// Raspberry: 192.168.43.150
// Local PC: 192.168.137.1

// LCD settings
// set the LCD number of columns and rows
int LCDColumns = 20;
int LCDRows = 4;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, LCDColumns, LCDRows);

// Block of memory that will be used for storing and manipulating the led data
// This sets up an array that we can manipulate to set/clear led data
CRGB leds[NUM_LEDS];

// Slimmemeter data, door API verstuur in JSON format, moet nog connectie opstellen tussen ESP en API
char JSONMessage[] = "{\"huidigVerbruik\":0.227,\"totVerbruikLaag\":2019.052,\"totVerbruikHoog\":2197.635,\"gasverbruik\":1983.182}";
float huidigVerbruik;
float totVerbruikLaag;
float totVerbruikHoog;
float gasverbruik;

// JSON Parse settings
StaticJsonBuffer<300> JSONBuffer;
JsonObject& parsed= JSONBuffer.parseObject(JSONMessage);

// MQTT settings
WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient);

void setup(){
  // Open serial connection, to show the result of the program and connect to the WiFi network
  Serial.begin(115200);
  
  WiFi.mode(WIFI_OFF);                            // Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);                            // This line hides the viewing of ESP as wifi hotspot

  // LCD setup
  lcd.init();                                     // Initialize LCD
  lcd.backlight();                                // turn on LCD backlight

  // Button setup
  pinMode(buttonGreen, INPUT);
  pinMode(buttonRed, INPUT);
  
  // Connection of the led, tells the library that the led is connected on pin 14 and how many leds there are
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);              // The led strip must stay off until there is a WiFi connection
  noWiFiConnection();                             // Call function all_leds_white to make the led strip white
  
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
  Serial.println(WiFi.localIP());                 // IP address assigned to your ESP

  // Show on LCD
  lcd.setCursor(0,0);
  lcd.printf("Connected to Network");
  delay(2000);
  lcd.clear();

  // Set the ServerName and MQTTPort
  //client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while(!client.connected()){
    Serial.println("Connecting to MQTT...");
 
    if(client.connect("ESP8266Client")){
       Serial.println("Connected to MQTT Broker");  
    } 
    else{
      Serial.println("failed with state ");
      Serial.println(client.state());
      delay(2000);
    }
  }
  client.publish("esp/slimmemeter", "publish test");
  client.subscribe("esp/slimmemeter");
}

void loop(){
  // Call the loop method of the PubSubClient, to allow the client to process 
  // the incomming messages and maintain its connection to the server
  client.loop();
  
  // Functions to show data on the LCD
  lcd_stroomverbruik();                           // Function to display the values of "stroomverbruik" on the lcd and in the Serial Monitor
  lcd_gasverbruik();                              // Function to display the values of "gasverbruik" on the lcd and in the Serial Monitor
  parseJSON();                                    // Function that parse the file it receives from the Apache server from JSON into float values
  buttonSimulation();                             // Function to simulate a fridge and tv screen
  algorithmLed();                                 // Function that decides what each individual led needs to display depending on the value of huidigVerbruik
  checkConnection();                              // Function that checks if the esp is still connected to the WiFi
}

void callback(char* topic, byte* payload, unsigned int length){ 
  // Handle message arrived
  String content = "";
  char character;
  for(int i = 0; i < length; i++){
      character = payload[i];
      content.concat(character);
  }

  Serial.println();
  Serial.println("*********************");
  Serial.println("Topic: ");
  Serial.println(topic);                          // Message published to broker and serial printed
  Serial.println("Content: ");
  Serial.println(content);                        // Message returned from broker and serial printed
  Serial.println("*********************");
  Serial.println();

  huidigVerbruik = content.toFloat();             // Convert the String content to a float named huidigVerbruik
  Serial.printf("test %.3f", huidigVerbruik);
  Serial.println();
  algorithmLed();
}

void parseJSON(){
  // Parsing 
  if(!parsed.success()){                            //Check for errors in parsing
    Serial.println("Parsing failed");
    delay(5000);
    return;
  }
  // char JSONMessage[] = "{\"huidigVerbruik\":0.227,\"totVerbruikLaag\":2019.052,\"totVerbruikHoog\":2197.635,\"gasverbruik\":1983.182}";
  huidigVerbruik = parsed["huidigVerbruik"];        // Get the value
  totVerbruikLaag = parsed["totVerbruikLaag"];      // Get the value
  totVerbruikHoog = parsed["totVerbruikHoog"];      // Get the value
  gasverbruik = parsed["gasverbruik"];              // Get the value
}

// Decides what happens to the leds
void algorithmLed(){
  // weinigVerbruik
  if(huidigVerbruik <= 0.08){
    weinigVerbruik();
  }
  // weinigMediumVerbruik
  if(huidigVerbruik > 0.08 && huidigVerbruik < 0.84){
    weinigMediumVerbruik();
  }
  // mediumVerbruik
  if(huidigVerbruik >= 0.84 && huidigVerbruik <= 1.24){
    mediumVerbruik();
  }
  // veelMediumVerbruik
  if(huidigVerbruik > 1.24 && huidigVerbruik < 2.0){
    veelMediumVerbruik();
  }
  // veelVerbruik
  if(huidigVerbruik >= 2.0){
    veelVerbruik();
  }
}

void lcd_stroomverbruik(){
  //Display of how much current is used at the moment and has been used today
  Serial.println("      Bewust-E");
  Serial.println("Stroomverbruik:");
  Serial.printf("Huidig: %.3f kWh\n", huidigVerbruik);
  Serial.printf("Dag:    %.3f kWh\n", totVerbruikHoog);

  // LCD Scherm
  lcd.setCursor(6,0);                             // Set cursor to third column, first row
  lcd.printf("Bewust-E");
  lcd.setCursor(0,1);                             // Set cursor to first column, second row
  lcd.print("Stroomverbruik:");
  lcd.setCursor(0,2);
  lcd.printf("Huidig: %.3f kWh", huidigVerbruik);
  lcd.setCursor(0,3);
  lcd.printf("Dag:    %.3f kWh", totVerbruikHoog);
  delay(3000);                                    // Refresh screen every 7 seconds
  lcd.clear();                                    // Clears the display to print new message
}

void lcd_gasverbruik(){
  //Display of how much gas has been used today
  Serial.println("      Bewust-E");
  Serial.println("Gasverbruik:");
  Serial.printf("Dag: %.3f m3\n", gasverbruik);
  
  // LCD Screen
  lcd.setCursor(6,0);                             // Set cursor to third column, first row
  lcd.printf("Bewust-E");
  lcd.setCursor(0,1);                             // Set cursor to first column, second row
  lcd.print("Gasverbruik:");
  lcd.setCursor(0,2);
  lcd.printf("Dag: %.3f m3", gasverbruik);
  delay(3000);                                    // Refresh screen every 7 seconds
  lcd.clear();                                    // Clears the display to print new message
}

void buttonSimulation(){
  // Read the state of the pushbutton value
  buttonStateGreen = digitalRead(buttonGreen);
  buttonStateRed = digitalRead(buttonRed);

  // Check if the buttons are pressed, if it is, the buttonState is HIGH
  if(buttonStateGreen == HIGH){
    Serial.println("Green button pressed");
    huidigVerbruik = 1.5;
  }
  if(buttonStateRed == HIGH){
    Serial.println("Red button pressed");
    huidigVerbruik = 2.3;
  }
}

// Function that checks if the esp is still connected to the WiFi
void checkConnection(){
  // Wait for connection
  while(WiFi.status() != WL_CONNECTED){
    noWiFiConnection();                           // Call function all_leds_white to make the led strip white to indicate that the connection has failed
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

/* Presets for the led strip */
// All Green
void weinigVerbruik(){
  leds[0]=CRGB(0,255,0);                          // GREEN
  leds[1]=CRGB(0,255,0);                          // GREEN
  leds[2]=CRGB(0,255,0);                          // GREEN
  leds[3]=CRGB(0,255,0);                          // GREEN
  leds[4]=CRGB(0,255,0);                          // GREEN
  leds[5]=CRGB(0,255,0);                          // GREEN
  leds[6]=CRGB(0,255,0);                          // GREEN
  leds[7]=CRGB(0,255,0);                          // GREEN
  FastLED.show();
}

// Green, Green, LightGreen, LightGreen, Yellow, Yellow, Orange, LightRed
void weinigMediumVerbruik(){
  leds[0]=CRGB(0,255,0);                          // GREEN
  leds[1]=CRGB(0,255,0);                          // GREEN
  leds[2]=CRGB(191,255,0);                        // LIGHT-GREEN
  leds[3]=CRGB(191,255,0);                        // LIGHT-GREEN
  leds[4]=CRGB(255,255,0);                        // YELLOW
  leds[5]=CRGB(255,255,0);                        // YELLOW
  leds[6]=CRGB(255,191,0);                        // ORANGE
  leds[7]=CRGB(255,64,0);                         // LIGHT-RED
  FastLED.show();
}

// Green, LightGreen, LightGreen, Yellow, Orange, LightRed, LighRed, Red
void mediumVerbruik(){
  leds[0]=CRGB(0,255,0);                          // GREEN
  leds[1]=CRGB(0,255,0);                          // GREEN
  leds[2]=CRGB(191,255,0);                        // LIGHT-GREEN
  leds[3]=CRGB(255,255,0);                        // YELLOW
  leds[4]=CRGB(255,191,0);                        // ORANGE
  leds[5]=CRGB(255,64,0);                         // LIGHT-RED
  leds[6]=CRGB(255,0,0);                          // RED
  leds[7]=CRGB(255,0,0);                          // RED
  FastLED.show();
}

// LightGreen, Yellow, Orange, Orange, LightRed, LightRed, Red, Red
void veelMediumVerbruik(){
  leds[0] = CRGB(191,255,0);                      // LIGHT-GREEN
  leds[1] = CRGB(255,255,0);                      // YELLOW
  leds[2] = CRGB(255,191,0);                      // ORANGE
  leds[3] = CRGB(255,191,0);                      // ORANGE
  leds[4] = CRGB(255,64,0);                       // LIGHT-RED
  leds[5] = CRGB(255,64,0);                       // LIGHT-RED
  leds[6] = CRGB(255,0,0);                        // RED
  leds[7] = CRGB(255,0,0);                        // RED
  FastLED.show();
}

// Alles Red
void veelVerbruik() {
  leds[0]=CRGB(255,0,0);                          // RED
  leds[1]=CRGB(255,0,0);                          // RED
  leds[2]=CRGB(255,0,0);                          // RED
  leds[3]=CRGB(255,0,0);                          // RED
  leds[4]=CRGB(255,0,0);                          // RED
  leds[5]=CRGB(255,0,0);                          // RED
  leds[6]=CRGB(255,0,0);                          // RED
  leds[7]=CRGB(255,0,0);                          // RED
  FastLED.show();
}

// Function turns all the leds white
void noWiFiConnection(){
  //leds[ID] = CRGB(red, green, blue) Only use value of 10 to reduce the amp draw of the strip when lit
  leds[0]=CRGB::White;                            // WHITE
  leds[1]=CRGB::White;                            // WHITE
  leds[2]=CRGB::White;                            // WHITE
  leds[3]=CRGB::White;                            // WHITE
  leds[4]=CRGB::White;                            // WHITE
  leds[5]=CRGB::White;                            // WHITE
  leds[6]=CRGB::White;                            // WHITE
  leds[7]=CRGB::White;                            // WHITE
  FastLED.show();
}
