//YOUR NODEMCU AND THE DEVICE ON WHICH THE LOCAL SERVER IS RUNNING MUST BE ON THE //SAME WIFI NETWORK

#include<ESP8266WiFi.h>

const char* ssid="spot";                // YourWiFiName
const char* password="?Spot!1234";      // YourWiFiPassword
const char* host="192.168.43.11";       //LOCAL IPv4 ADDRESS...ON CMD WINDOW TYPE ipconfig/all
const uint16_t port=80;                 //PORT OF THE LOCAL SERVER
int rank=4;                             //THE RANK WHOSE DATA YOU WANT TO FETCH

void setup() {
  Serial.begin(115200);
  Serial.println("STARTING CONNECTION TO WIFI");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED) {  //waiting for device to be connected to the network
    Serial.print(".");
    delay(500);
  }
  Serial.println("CONNECTED TO WIFI");
  Serial.print("IP:");
  Serial.println(WiFi.localIP());
}

void loop(){
  String l="";
  String lengthh="";
  Serial.print("Connecting to: ");
  Serial.print(host);
  Serial.print(":");
  Serial.println(port);
  WiFiClient client;
  if(!client.connect(host,port)) {      //IF THIS OCCURS MAKE THEN FIREWALL IS STOPPING THE CONNECTION OR THE IP ADDRESS/PORT OF THE SERVER IS INCORRECT.
    Serial.println("CONNECTION FAILED");
    delay(4000);
    return;
  }
  else{
      Serial.println("CONNECTED TO THE SERVER");
      client.print(String("GET ")+"/test2.php?RANK="+rank+" HTTP/1.1\r\n"+"Host:"+host+"\r\n"+"Connection:close\r\n\r\n");
      unsigned long timeout=millis();
      while(client.available()==0){ 
        //close the connection if it has been connected for more than 5 seconds
        if(millis()-timeout>5000){
          Serial.println(">>> Client Timeout !");
          client.stop();
          delay(5000);
          return;
        }
      }
    Serial.println("receiving from remote server");
    
    //Start reading the response from the server
    while(client.available()) {
      char ch = static_cast<char>(client.read());
      Serial.print(ch);
    }
    // Close the connection
    Serial.println();
    Serial.println("closing connection");
    client.stop();
    delay(6000);
  }
}
