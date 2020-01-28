/*=======================================================================================================
 * Author:  Appie Strorman
 * Project: The Challenge
 * Group:   D3D
 * Name:    ESP BaroMeter
 * Date:    1/28/2020
 * Version: V2.0
 * 
 * In this version the code:
 *  - LCD connection.
 *  - Led Strip connection.
 *  - If the WiFi connection fails, then the leds turn white and the lcd displays "Reconnecting to WiFi...".
 *  - Connection with the mqtt broker between the Raspberry Pi Zero W and the ESP that's running local.
 *  - Algortihm to display the correct led color.
 *  - Live presentation (10 secs) of the kWh on the LCD and on the Serial Monitor.
 *  - Two buttons, red is the simulation of a washing machine, green is the simulation of a tv.
 *  - Need to hold the buttons because the program only checks the values every 10 seconds.
 *  - Sends HTTP request to the Apache2 server using php.
 *  - Parse the received data from the http request.
 *  - Multiple delays: so the lcd can refresh every 10 seconds, a php post request is send every 10 seconds and the buttons will listen all the time.
 *  
 * Setup ESP using Arduino IDE:
 *  - TOOLS/BOARD/"Adaruit Feather HUZZAH ESP8266"
 *  - Additional Boards Manager URL (Paste): http://arduino.esp8266.com/stable/package_esp8266com_index.json
 * 
 * Libraries:
 *  - ArduinoJSOn:         https://github.com/bblanchon/ArduinoJson/archive/v5.13.5.zip
 *  - ESP8266HTTPClient:   https://codeload.github.com/manrueda/ESP8266HttpClient/zip/master
 *  - ESP8266WiFi:         https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi
 *  - FastLED:             https://codeload.github.com/FastLED/FastLED/zip/master
 *  - Liquid Crystal:      https://github.com/marcoschwartz/LiquidCrystal_I2C/archive/master.zip
 *  - PubSubClient(MQTT):  https://codeload.github.com/knolleary/pubsubclient/zip/v2.
 *  - VitrualDelay:        https://github.com/avandalen/VirtualDelay
 *  - Streaming:           https://www.arduinolibraries.info/libraries/streaming
 *  
 *
 * Documentation:
 *  - https://github.com/FastLED/FastLED/wiki/Basic-usage
 *  - https://randomnerdtutorials.com/decoding-and-encoding-json-with-arduino-or-esp8266/
 *  - https://www.avdweb.nl/arduino/timing/virtualdelay
 * 
 * Color-picker:
 *  - https://www.w3schools.com/colors/colors_picker.asp
 *
 * Connection LCD:
 * ---------------
 * LCD  ->  ESP
 * ---------------
 * GND  ->  GND
 * VCC  ->  VIN
 * SDA  ->  D2
 * SCL  ->  D1
 * ---------------
=======================================================================================================*/

// Libraries to be able to connect the esp8266 to a WiFi network, let the LCD and LED work properly and allow us to connect to a MQTT broker and publish/subscribe messages in topics
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <FastLED.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <avdweb_VirtualDelay.h>
#include <Streaming.h>

// Led Strip macro definitions
#define NUM_LEDS 8                                // Number of leds: 8
#define DATA_PIN 14                               // Connected to GPIO 14, NodeMCU pin D5
#define BRIGHTNESS 120                            // Sets the brightness of the leds between 0-255 

// Buttons definitions
#define buttonGreen 12                            // D6 on esp
#define buttonRed 13                              // D7 on esp
int buttonStateGreen = 0;
int buttonStateRed = 0;

// WiFi settings
const char* ssid = "HotspotDaan";                 // YourWiFiName
const char* password = "tireda1234";              // YourWiFiPassword
const char* url = "http://192.168.2.6/php/post_req.php";  // Apache2 server address (raspberry ip)
const char* mqtt_server = "192.168.137.1";        // mqtt_server address
// Raspberry: 192.168.43.150
// Local PC: 192.168.137.1

// LCD settings
int LCDColumns = 20;
int LCDRows = 4;
LiquidCrystal_I2C lcd(0x27, LCDColumns, LCDRows); // Set the LCD address to 0x27 for a 16 chars and 2 line display

// Block of memory that will be used for storing and manipulating the led data
// This sets up an array that we can manipulate to set/clear led data
CRGB leds[NUM_LEDS];

// MQTT settings
WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient);

// default = millis
VirtualDelay delay1, delay2, delay3;

// Slimmemeter data, door php post reques verstuurd
String payload;
char json[139];
float huidigVerbruik;
float totVerbruikLaag;
float totVerbruikHoog;
float gasVerbruik;

void setup(){
  // Open serial connection, to show the result of the program and connect to the WiFi network
  Serial.begin(115200);
  
  WiFi.mode(WIFI_OFF);                              // Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);                              // This line hides the viewing of ESP as wifi hotspot

  // LCD setup
  lcd.init();                                       // Initialize LCD
  lcd.backlight();                                  // turn on LCD backlight

  // Button setup
  pinMode(buttonGreen, INPUT);
  pinMode(buttonRed, INPUT);
  
  // Connection of the led, tells the library that the led is connected on pin 14 and how many leds there are
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);                // The led strip must stay off until there is a WiFi connection
  noWiFiConnection();                               // Call function all_leds_white to make the led strip white
  
  // Connect to the WIFI
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  
  // While wifi not connected yet, print '.' every 1 sec, 
  // then after it connected, get out of the loop
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connection OK!");
  Serial.println();
  
  //If connection is successful, show IP address in serial monitor
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());                   // IP address assigned to your ESP

  // Signalstrength
  long rssi = WiFi.RSSI();
  Serial.print("Signaal sterkte (RSSI): ");
  Serial.print(rssi);
  Serial.println("dBm");
  Serial.println("");

  // Show on LCD
  lcd.setCursor(0,0);
  lcd.printf("Connected to Network");
  delay(2000);  
  lcd.clear();

  // Set the ServerName and MQTTPort
  // client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while(!client.connected()){
    Serial.println("Connecting to MQTT...");
 
    if(client.connect("ESP8266Client")){
       Serial.println("Connected to MQTT Broker");
       Serial.println();
    } 
    else{
      Serial.println("failed with state ");
      Serial.println(client.state());
      Serial.println();
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

  // Function to connect to the server and send a post request
  httpConnection();

  // Simulate a fridge and tv screen
  buttonSimulation();

  // Breaks the deadlock, you can start with any delay object you want e.g. delay2.start(0);
  DO_ONCE(delay1.start(0));

  // One time only delay for the program to startup so everything is loaded properly
  boolean doneOnce = false;
  if(!doneOnce){
      // Wait for 10 seconds
      delay(10000);
      doneOnce = true;
  }

  if(delay1.elapsed()){
    // Display the consumption on the lcd and in the Serial Monitor
    Serial.println("      Bewust-E");
    Serial.println("Stroomverbruik:");
    Serial.printf("Huidig: %.3f kWh\n", huidigVerbruik);
    Serial.printf("Dag:    %.3f kWh\n", totVerbruikHoog);
    Serial.println();
    
    // LCD screen
    lcd.setCursor(6,0);                             // Set cursor to third column, first row
    lcd.printf("Bewust-E");
    lcd.setCursor(0,1);                             // Set cursor to first column, second row
    lcd.printf("Stroomverbruik:");
    lcd.setCursor(0,2);
    lcd.printf("Huidig: %.3f kWh", huidigVerbruik);
    lcd.setCursor(0,3);
    lcd.printf("Dag:    %.3f kWh", totVerbruikHoog);
  
    // Start delay2 in 10 sec, shows the "stroomverbruik" on the LCD and in the Serial Monitor
    delay2.start(10000);
  }
  if(delay2.elapsed()){
    lcd.clear();                                    // Clears the display to print new message
  
    //Display of how much gas has been used today
    Serial.println("      Bewust-E");
    Serial.println("Gasverbruik:");
    Serial.printf("Dag: %.3f m3\n", gasVerbruik);
    Serial.println(); 
  
    // LCD screen
    lcd.setCursor(6,0);                             // Set cursor to third column, first row
    lcd.printf("Bewust-E");
    lcd.setCursor(0,1);                             // Set cursor to first column, second row
    lcd.print("Gasverbruik:");
    lcd.setCursor(0,2);
    lcd.printf("Dag: %.3f m3", gasVerbruik);
    
    // Start delay1 in 10 sec, shows the "stroomverbruik" on the LCD and in the Serial Monitor
    delay1.start(10000);
  }
  
  // Function that decides what each individual led needs to display depending on the value of huidigVerbruik
  algorithmLed();

  // Function that checks if the esp is still connected to the WiFi
  checkConnection();
}

void buttonSimulation(){
  // Simulate a fridge and tv screen
  // Read the state of the pushbutton value
  buttonStateGreen = digitalRead(buttonGreen);
  buttonStateRed = digitalRead(buttonRed);
  
  // Check if the buttons are pressed, if it is, the buttonState is HIGH
  if(buttonStateGreen == HIGH){
    huidigVerbruik = 1.5;
    algorithmLed();
  }
  if(buttonStateRed == HIGH){
    huidigVerbruik = 2.3;
    algorithmLed();
  }
}

void callback(char* topic, byte* payload, unsigned int length){
  // Callback variables
  String content = "";
  char character;
  
  // Handle message arrived
  for(int i = 0; i < length; i++){
      character = payload[i];
      content.concat(character);
  }
  
  // Print the data 
  Serial.println();
  Serial.println("------ NEW DATA FROM MQTT -----");
  
  Serial.println("Topic: ");
  Serial.println(topic);                            // Message published to broker and serial printed
  Serial.println("Content: ");
  Serial.println(content);                          // Message returned from broker and serial printed
  
  Serial.println("-------------------------------");
  Serial.println();

  // Set huidigVerbruik to the received data value
  huidigVerbruik = content.toFloat();               // Convert the String content to a float named huidigVerbruik
}

// Function that parse the received json file
void handleReceivedMessage(String json){
  // JSON Parse settings
  StaticJsonBuffer<250> JSONBuffer;                   // Memory pool
  JsonObject& parsed= JSONBuffer.parseObject(json);   // Parse json
 
  if(!parsed.success()){   //Check for errors in parsing
    Serial.println("Parsing failed");
    return;
  }

  // Get the values
  char tijd = parsed["tijd"];
  huidigVerbruik = parsed["huidigVerbruik"];
  totVerbruikLaag = parsed["totVerbruikLaag"];
  totVerbruikHoog = parsed["totVerbruikHoog"];
  gasVerbruik = parsed["gasVerbruik"];

  // Print the data 
  Serial.println();
  Serial.println("----- NEW DATA FROM CLIENT ----");

  Serial.printf("tijd: %d", tijd);
  Serial.println();
  Serial.printf("huidigVerbruik: %0.3f", huidigVerbruik);
  Serial.println();
  Serial.printf("totVerbruikLaag: %0.3f", totVerbruikLaag);
  Serial.println();
  Serial.printf("totVerbruikHoog: %0.3f", totVerbruikHoog);
  Serial.println();
  Serial.printf("gasVerbruik: %0.3f", gasVerbruik);
  Serial.println();
  
  Serial.println("-------------------------------");
  Serial.println();
}

// Function to connect to the server and send a post request
void httpConnection(){
  // Delay for 10 seconds
  delay3.start(10000);

  if(delay3.elapsed()){
    if(WiFi.status() == WL_CONNECTED){      // Check WiFi connection status
      HTTPClient http;                      // Declare an object of class HTTPClient
      
      http.begin(url);                      // Specify request destination
      int httpCode = http.GET();            // Send the request
      
      if(httpCode > 0){                     // Check the returning code
        payload = http.getString();         // Get the request response payload
        Serial.println("payload");
        Serial.println(payload);            // Print the response payload
        handleReceivedMessage(payload);     // Pass the payload to the function handleReceivedMessage to parse the json object
      }
      http.end();                           // Close connection
    }
    else{
      Serial.println("Error in WiFi Connection");
    }
  }
}

// Function that decides what each individual led needs to display depending on the value of huidigVerbruik
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

// Function that checks if the esp is still connected to the WiFi
void checkConnection(){
  // Wait for connection, [doesn't auto reconnect, need to reset esp]
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

// Presets for the led strip
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

// All Red
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

// Turns all the leds white
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
