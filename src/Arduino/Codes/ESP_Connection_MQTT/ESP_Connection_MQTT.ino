/*
 * PubSubClient Library:
 * https://github.com/knolleary/pubsubclient/releases/tag/v2.7
 */

// Libraries to be able to connect the esp8266 to a WiFi network and allow us to connect to a MQTT broker and publish/subscribe messages in topics
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Variables to connect esp to a network
const char* ssid="spot";                          // YourWiFiName
const char* password="?Spot!1234";                // YourWiFiPassword

// Information and credentials of the MQTT server
const char* mqttServer = "tailor.cloudmqtt.com";  // YourMgttServer/IP pc
const int mqttPort = 11261;                       // YourMqttPort
const char* mqttUser = "shyoviuu";                // YourMqttUser
const char* mqttPassword = "Z3sfnnvn3VZU";        // YourMqttUserPassword

WiFiClient espClient;
PubSubClient client(espClient);

void setup(){
  // Open serial connection, to show the result of the program and connect to the WiFi network
  Serial.begin(115200);
  WiFi.begin(ssid,password);
   
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Connecting to WiFi..");
  } 
  Serial.println("Connected to the WiFi network");
  
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while(!client.connected()){
    Serial.println("Connecting to MQTT...");
 
    if(client.connect("ESP8266Client", mqttUser, mqttPassword )){
       Serial.println("connected");  
    } 
    else{
      Serial.println("failed with state ");
      Serial.println(client.state());
      delay(2000);
    }
  }
  client.publish("esp/slimmemeter", "001911.406*kWh");
  client.subscribe("esp/slimmemeter");
}

void callback(char* topic, byte* payload, unsigned int length){ 
  // Handle message arrived
  String content = "";
  char character;
  for(int i = 0; i < length; i++) {
      character = payload[i];
      content.concat(character);
  }

  Serial.println("Topic: ");
  Serial.println(topic);
  Serial.println("Content: ");
  Serial.println(content);      // message sent out by button actions is returned from broker and serial printed

  // Set specific virtual switches on basis of specific incoming messages
  if(content <= "0.005*kWh"){
    Serial.println("GROEN, GROEN, GROEN");
  }
  if(content >= "0.005*kWh"){
    Serial.println("ROOD, ROOD, ROOD");
  }
}

// Call the loop method of the PubSubClient, to allow the client to process the incomming messages and maintain its connection to the server
void loop(){
  client.loop();
}
