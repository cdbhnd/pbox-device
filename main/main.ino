#include <LGPS.h>
#include <LGPRS.h>
#include <LGPRSClient.h>
#include <LGPRSServer.h>
#include <LGPRSUdp.h>
#include <LTask.h>
#include <LWiFi.h>
#include <LWiFiClient.h>
#include <PubSubClient.h>
#include <ATT_IOT.h>
#include <Grove_LED_Bar.h>
#include <LBattery.h>
#include <DHT.h>
#include <Wire.h>
#include <ADXL345.h>
#include <LFlash.h>
#include <LSD.h>
#include <LStorage.h>

///////// ATT_VARIABLES /////////
#define httpServer "api.AllThingsTalk.io"                  
#define mqttServer "broker.smartliving.io"

// char deviceId[] = "tIfnBDpabrH4K6aF5l2RCAnv"; // box_1
// char deviceId[] = "tBulZIcRpJqgrDcD2EoWxvrf"; // box_2
char clientId[] = "ognjent_EjGwg4VW";
char clientKey[] = "GsqcBmey";
ATTDevice Device(deviceId, clientId, clientKey);

void callback(char* topic, byte* payload, unsigned int length);
LGPRSClient c;
PubSubClient pubSub(mqttServer, 1883, callback, c);

int gpsId = 2;        // GPS sensor
int actId = 3;        // Switch actuator
int dhtId = 4;        // DHT sensor
int batId = 5;        // Battery
int accId = 6;        // Accelerometer
int logId = 7;        // Log 

///////// LED /////////
Grove_LED_Bar bar(9, 8, 0);

///////// FLAGS /////////
int emitting;         // Emitting data to ATT
int fixed;            // Satellites fix position
int logging;          // Logging data
int batLevel;       
int batCharging;

///////// LOOP /////////
int interval;
int fixingDelay = 3000;
int emittingDelay = 10000;
unsigned long newTime;
unsigned long curTime;

void setLoopInterval(int sec) 
{
  interval = sec;
};
//////////////////////////////////

void setup()
{
  initializeBoard();
  initializeLog();
  initializeLED();  
  initializeGPS();
//  initializeWIFI();
// initializeGPRS();
  initializeDHT();
// initializeAccelerometer();
  initializeATT();
}

void loop()
{
  curTime = millis();
  
  checkBatteryStatus();
  
  if (emitting == 1) {
    if (curTime > (newTime + interval))
    {
      emitBattery();
      emitGPS();
      emitDHT();
//    emitAcceleration();
      
      if(logging == 1)
      {
        logData();  
      };
      
      newTime = curTime; 
    };
  } else {
    turnOffLedBar();
  };

  Device.Process(); 
}

