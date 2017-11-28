#include <ESP8266WiFi.h>
#include <PubSubClient.h>   // Read the rest of the article
#include <stdlib.h>
#include "RunningAverage.h"
RunningAverage myRA(10);
int samples = 0;

const char *ssid =  "Buddhima";   // cannot be longer than 32 characters!
const char *pass =  "buddhima";   //
const char* server = "api.thingspeak.com";
String apiKey ="TCWVCUE5K5G6YHNO";
float ratio;
int state=0;
int decision=0;
int previousDecision=0;
char from;
   

#define BUFFER_SIZE 100

unsigned long previousMillis = 0;
const long interval = 10000;



void setup() {
  pinMode(D0,INPUT);
  
  Serial.begin(115200);  //set the baud rate
  
  delay(10);
  Serial.println();
  Serial.println();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {  //wifi not connected?
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED){
      Serial.println("...");
      delay(500);
    }  
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }

  if (WiFi.status() == WL_CONNECTED) {
    //client object makes connection to server

    reading();
    
 

  
  }
 

}
void sendValueTS(int value)
{  
   WiFiClient client;
  
   if (client.connect(server, 80)) { // use ip 184.106.153.149 or api.thingspeak.com
   Serial.println("WiFi Client connected ");
   
   String postStr = apiKey;
   postStr += "&field1=";
   postStr += String(value);
  
   
   client.print("POST /update HTTP/1.1\n");
   client.print("Host: api.thingspeak.com\n");
   client.print("Connection: close\n");
   client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
   client.print("Content-Type: application/x-www-form-urlencoded\n");
   client.print("Content-Length: ");
   client.print(postStr.length());
   client.print("\n\n");
   client.print(postStr);
   delay(10000);
   
   }//end if
 
 client.stop();
}//end send

void reading() {
  int data = analogRead(A0);
  myRA.addValue(data);
  samples++;


  Serial.print("value: ");
  Serial.println(data);

  if (samples == 100)
  {
    Serial.print("Running Average: ");
    Serial.println(myRA.getAverage(), 3);
    if (myRA.getAverage() > 150) {
      sendValueTS(1);
    } else {
      sendValueTS(0);
    }

    samples = 0;
    myRA.clear();
  }
  delay(100);

}


