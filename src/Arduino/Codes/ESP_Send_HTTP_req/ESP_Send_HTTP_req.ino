/*
 * https://github.com/manrueda/ESP8266HttpClient
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
 
// WiFi parameters to be configured
const char* ssid = "HotspotDaan";         // YourWiFiName
const char* password = "tireda1234";      // YourWiFiPassword
 
const char* http_site = "localhost:8080/index.php";
// test: http://jsonplaceholder.typicode.com/users

int count = 0;

void setup(void){ 
  Serial.begin(115200);
  Serial.print("Bezig met verbinden");
  WiFi.begin(ssid, password);             // Connect to WiFi
 
  // while wifi not connected yet, print '.'
  // then after it connected, get out of the loop
  while(WiFi.status() != WL_CONNECTED){
     delay(500);
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
 // Serial.println(count);
  if(WiFi.status() == WL_CONNECTED){      // Check WiFi connection status
    HTTPClient http;                      // Declare an object of class HTTPClient
    
    http.begin(http_site);                // Specify request destination
    int httpCode = http.GET();            // Send the request
    
    if(httpCode > 0){                     // Check the returning code
      String payload = http.getString();  // Get the request response payload
      Serial.println(payload);            // Print the response payload
    }
    http.end();                           // Close connection
  }
  else{
    Serial.println("Error in WiFi Connection");
  }
//  delay(10000);                           // Send a request every 30 seconds   
//  count++;
}
