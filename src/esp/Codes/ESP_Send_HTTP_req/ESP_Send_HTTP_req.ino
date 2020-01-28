/*
 * https://github.com/manrueda/ESP8266HttpClient
 * https://www.phptpoint.com/getting-data-from-mysql-into-json-using-php/
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
 
// WiFi parameters to be configured
const char* ssid = "HotspotDaan";         // YourWiFiName
const char* password = "tireda1234";      // YourWiFiPassword
const char* url = "http://192.168.2.6/php/post_req.php";

// Slimmemeter data, door php post reques verstuurd
String payload;
char json[139];
float huidigVerbruik;
float totVerbruikLaag;
float totVerbruikHoog;
float gasVerbruik;

void handleReceivedMessage(String json){
  // JSON Parse settings
  StaticJsonBuffer<250> JSONBuffer;                   // Memory pool
  JsonObject& parsed= JSONBuffer.parseObject(json);   // Parse json
 
  if(!parsed.success()){   //Check for errors in parsing
    Serial.println("Parsing failed");
    return;
  }

  huidigVerbruik = parsed["huidigVerbruik"];      // Get the value
  totVerbruikLaag = parsed["totVerbruikLaag"];    // Get the value
  totVerbruikHoog = parsed["totVerbruikHoog"];    // Get the value
  gasVerbruik = parsed["gasVerbruik"];            // Get the value

  Serial.println();                               // Next line
  Serial.println("----- NEW DATA FROM CLIENT ----");
  
  Serial.printf("huidigVerbruik: %0.3f", huidigVerbruik);
  Serial.println();                               // Next line
  Serial.printf("totVerbruikLaag: %0.3f", totVerbruikLaag);
  Serial.println();                               // Next line
  Serial.printf("totVerbruikHoog: %0.3f", totVerbruikHoog);
  Serial.println();                               // Next line
  Serial.printf("gasVerbruik: %0.3f", gasVerbruik);
  Serial.println();                               // Next line
  
  Serial.println("-------------------------------");
}

void setup(){ 
  Serial.begin(115200);
  Serial.print("Bezig met verbinden");
  WiFi.begin(ssid, password);                     // Connect to WiFi
 
  // while wifi not connected yet, print '.'
  // then after it connected, get out of the loop
  while(WiFi.status() != WL_CONNECTED){
     delay(1000);
     Serial.print(".");
  }
  
  // Verbonden.
  Serial.println("OK!");
  
  // Access Point (SSID).
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  
  // IP adres.
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
 
  // Signaalsterkte.
  long rssi = WiFi.RSSI();
  Serial.print("Signaal sterkte (RSSI): ");
  Serial.print(rssi);
  Serial.println("dBm");
  Serial.println("");
}
 
void loop(){
  if(WiFi.status() == WL_CONNECTED){      // Check WiFi connection status
    HTTPClient http;                      // Declare an object of class HTTPClient
    
    http.begin(url);                      // Specify request destination
    int httpCode = http.GET();            // Send the request
    
    if(httpCode > 0){                     // Check the returning code
      payload = http.getString();         // Get the request response payload
      Serial.println(payload);            // Print the response payload
      handleReceivedMessage(payload);     // Pass the payload to the function handleReceivedMessage to parse the json object
    }
    http.end();                           // Close connection
  }
  else{
    Serial.println("Error in WiFi Connection");
  } 
  delay(10000);                           // Send a request every 10 seconds
}
