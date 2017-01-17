/*
   Copyright 2014-2016 AllThingsTalk
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/
#include <LGPS.h>
#include <LGPRS.h>
#include <LGPRSClient.h>
#include <LGPRSServer.h>
#include <LGPRSUdp.h>
//#include <LTask.h>
#include <PubSubClient.h>
#include <ATT_IOT.h>

char deviceId[] = "tIfnBDpabrH4K6aF5l2RCAnv";
char clientId[] = "ognjent_EjGwg4VW";
char clientKey[] = "GsqcBmey";

int a0=A0;
int a0Id=0;
int a1=8;
int a1Id=1;
int a2=1;
int a2Id=2;
struct Locations {
  int satellite_status;
  double latitude;
  double longitude;
};
struct Locations gpsData;
ATTDevice Device(deviceId, clientId, clientKey);                //create the object that provides the connection to the cloud to manager the device.
#define httpServer "api.AllThingsTalk.io"                  // HTTP API Server host                  
#define mqttServer httpServer                               // MQTT Server Address 
//required for the device
void callback(char* topic, byte* payload, unsigned int length);
LGPRSClient c;
PubSubClient pubSub(mqttServer, 1883, callback, c);

void setup()
{
  initializeDevice();
  Serial.begin(115200);
  while(!Serial);                                                       //for the linkit, we need to wait until the serial monitor is initialized correctly, if we don't do this, we don't get to see the logging.
                                                                        //Warning for battery and mains power usage, comment out the above line, otherwise the setup() will continue to wait for the Serial to become available
  Serial.println("starting");
  pinMode(a1, OUTPUT);
  
  while(!LGPRS.attachGPRS("NET","telenor","gprs"))
  {
    Serial.println("connecting");
    delay(1000);
  }
  Serial.println("connected");
  while(!Device.Connect(&c, httpServer))                        // connect the device with the IOT platform.
    Serial.println("retrying");
  Device.AddAsset(a0Id, "knob", "rotary switch",false, "integer");
  Device.AddAsset(a1Id, "led", "light emitting diode", true, "boolean");
  Device.AddAsset(a2Id, "gps", "gps sensor", false, "string");
  while(!Device.Subscribe(pubSub))                                      // make certain that we can receive message from the iot platform (activate mqtt)
    Serial.println("retrying"); 
  digitalWrite(a1, LOW);
}
unsigned long time;
void loop()
{
  unsigned long curTime = millis();
  if (curTime > (time + 20000))                                          // publish light reading every 5 seconds to sensor 1
  {
    gpsData = getGpsData();
    int length = 40;
    char* cords = new char[length];
    sprintf(cords, "%f,%f",gpsData.latitude, gpsData.longitude);
    Device.Send(String(cords), a2Id);
    
    time = curTime; 
  };
  Device.Process(); 
}
// Callback function: handles messages that were sent from the iot platform to this device.
void callback(char* topic, byte* payload, unsigned int length) 
{ 
  String msgString; 
  {                                                            //put this in a sub block, so any unused memory can be freed as soon as possible, required to save mem while sending data
    char message_buff[length + 1];                        //need to copy over the payload so that we can add a /0 terminator, this can then be wrapped inside a string for easy manipulation.
    strncpy(message_buff, (char*)payload, length);        //copy over the data
    message_buff[length] = '\0';                      //make certain that it ends with a null           
          
    msgString = String(message_buff);
    msgString.toLowerCase();                  //to make certain that our comparison later on works ok (it could be that a 'True' or 'False' was sent)
  }
  int* idOut = NULL;
  {                                                       //put this in a sub block, so any unused memory can be freed as soon as possible, required to save mem while sending data
    int pinNr = Device.GetPinNr(topic, strlen(topic));
    
    Serial.print("Payload: ");                            //show some debugging.
    Serial.println(msgString);
    Serial.print("topic: ");
    Serial.println(topic);
    
    if (pinNr == a1Id)
    {
      if (msgString == "false") {
        Serial.println("LED off");  
            digitalWrite(a1, LOW);           //change the LED status to false
            idOut = &a1Id;
                              
      }
      else if (msgString == "true") {
        Serial.println("LED on"); 
        digitalWrite(a1, HIGH);              //change the LED status to true
            idOut = &a1Id;
           
      }
    }
  }
  if(idOut != NULL)                //also let the iot platform know that the operation was succesful: give it some feedback. This also allows the iot to update the GUI's correctly & run scenarios.
    Device.Send(msgString, *idOut);    
}

