/*
 Base on code by Igor Jarc
 which was based on https://github.com/DennisSc/easyIoT-ESPduino/blob/master/sketches/ds18b20.ino
 
 External libraries:
 - https://github.com/milesburton/Arduino-Temperature-Control-Library
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
extern "C" {
#include "user_interface.h" //for sleep
}



//AP definitions - update this
#define AP_SSID     "SSID"
#define AP_PASSWORD "SECRET_KEY"

#define ROOM "upstairs"
#define REPORT_INTERVAL 300 // in sec

#define SERVER_ADDRESS     "192.168.0.12"
#define SERVER_PORT        8000



#define ONE_WIRE_BUS 2  // DS18B20 pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);


float oldTemp;


void wifiConnect()
{
    Serial.print("Connecting to AP");
    WiFi.begin(AP_SSID, AP_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");  
}

void sendTemperature(float temp)
{  
   WiFiClient client;
   
   while(!client.connect(SERVER_ADDRESS, SERVER_PORT)) {
    Serial.println("connection failed");
    wifiConnect(); 
  }

  String url = "";
  // URL: "/Temperature/room/[room]/temp/[temperature]"
  url += "/Temperature/room/"+ String(ROOM) + "/value/"+String(temp);

  Serial.print("POST data to URL: ");
  Serial.println(url);
  
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + String(SERVER_ADDRESS) + "\r\n" + 
               "Connection: close\r\n" + 
               "Content-Length: 0\r\n" + 
               "\r\n");

  delay(100);
    while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("Connection closed");
}

void setup() {
  Serial.begin(115200);
  
  wifiConnect();
    
  oldTemp = -1;
}

void loop() {
  float temp;
  
  do {
    DS18B20.requestTemperatures(); 
    temp = DS18B20.getTempCByIndex(0);
    Serial.print("Temperature: ");
    Serial.println(temp);
  } while (temp == 85.0 || temp == (-127.0));
  
  if (temp != oldTemp)
  {
    sendTemperature(temp);
    oldTemp = temp;
  }
  
  //then, to sleep:
  system_deep_sleep_set_option(0);
  system_deep_sleep(REPORT_INTERVAL*1000000); //in us, 1 min  
}