#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
 
// Variables to connect the esp to a network
const char* ssid = "HotspotDaan";           // YourWiFiName
const char* password = "tireda1234";        // YourWiFiPassword
const char* url = "192.168.2.6/php/post_req.php";
 
void setup (){
  Serial.begin(115200);

  WiFi.mode(WIFI_OFF);                      // Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);                      // This line hides the viewing of ESP as wifi hotspot
  WiFi.begin(ssid, password);               // Connect to WiFi
   
  while(WiFi.status() != WL_CONNECTED){ 
    delay(5000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void loop(){
  if(WiFi.status() == WL_CONNECTED){        //Check WiFi connection status
    HTTPClient http;                        //Declare an object of class HTTPClient
  
    http.begin(url);                        //Specify request destination
    int httpCode = http.GET();              //Send the request
  
    if(httpCode > 0){                       //Check the returning code
      String payload = http.getString();    //Get the request response payload
      Serial.println(payload);              //Print the response payload
    }
    Serial.print("httpCode:   ");
    Serial.println(httpCode);
    http.end();                             //Close connection
  }
  delay(10000);                             //Send a request every 10 seconds
}
