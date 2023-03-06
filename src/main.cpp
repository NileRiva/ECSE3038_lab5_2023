#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"

const char* endpoint ="https://ecse-three-led-api-v2.onrender.com/api/state" ;


const int led1 = 15;
const int led2 = 2;
const int led3 = 4;
int i=0;
bool ledstate[8][3]= {  {false,false,false},
                        {false,false,true},
                        {false,true,false},
                        {false,true,true},
                        {true,false,false},
                        {true,false,true},
                        {true,true,false},
                        {true,true,true}};

void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("The Bluetooth Device is Ready to Pair");
  Serial.println("Connected @");
  Serial.print(WiFi.localIP());
}

void loop() {
  
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
    String http_response;


    String route = endpoint;
    http.begin(route);
    http.addHeader("X-API-Key", API_KEY);
    http.addHeader("Content-Type", "application/json");
    StaticJsonDocument<1024> doc; // Empty JSONDocument
    String httpRequestData; // Emtpy string to be used to store HTTP request data string

    doc["light_switch_1"]=ledstate[i][0];
    doc["light_switch_2"]=ledstate[i][1];
    doc["light_switch_3"]=ledstate[i][2];
    serializeJson(doc, httpRequestData);

    int httpResponseCode = http.PUT(httpRequestData);


    if (httpResponseCode>0) {
        Serial.print("Response:");
        Serial.print(httpResponseCode);
        http_response = http.getString();
        Serial.println(http_response);}
      else {
        Serial.print("Error: ");
        Serial.println(httpResponseCode);}
      
      http.end();
      
      
      digitalWrite(led1,ledstate[i][0]);
      digitalWrite(led2,ledstate[i][1]);
      digitalWrite(led3,ledstate[i][2]);
      if(i<7){i++;}
      else{i=0;}
  
      Serial.println("Light 1:");
      Serial.println(ledstate[i][0]);
      Serial.println("Light 2:");
      Serial.println(ledstate[i][1]);
      Serial.println("Light 3:");
      Serial.println(ledstate[i][2]);
      
      Serial.println("Lights Switched Successfully");
      delay(2000);   
  }
  else {return;}
}