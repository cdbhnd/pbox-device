#include <LGPS.h>
#include <LGPRS.h>
#include <LGPRSClient.h>
#include <LGPRSServer.h>
#include <LGPRSUdp.h>
#include <PubSubClient.h>
#include <ATT_IOT.h>

#define httpServer "api.AllThingsTalk.io"                  
#define mqttServer "broker.smartliving.io" 

///////// DEVICE_VARIABLES /////////
char deviceId[] = "tIfnBDpabrH4K6aF5l2RCAnv";
char clientId[] = "ognjent_EjGwg4VW";
char clientKey[] = "GsqcBmey";
ATTDevice Device(deviceId, clientId, clientKey);

///////// ASSETS_ID'S /////////
int id = 2;           // GPS sensor
int actId = 3;        // Switch actuator

///////// FLAGS /////////
int emitting;         
int fixed;            // Satellites fix position

///////// LOOP_VARIABLES /////////
int interval;
int offDelay = 3000;
int fixingDelay = 1000;
int emittingDelay = 20000;
unsigned long newTime;
unsigned long curTime;

///////// GPS_VARIABLES /////////
gpsSentenceInfoStruct info;
int satellite_status;
double latitude;
double longitude;
String gpsCoords;

///////// GPRS_VARIABLES /////////
char apn[] = "NET";
char username[] = "telenor";
char password[] = "gprs";
//////////////////////////////////

void callback(char* topic, byte* payload, unsigned int length);
LGPRSClient c;

PubSubClient pubSub(mqttServer, 1883, callback, c);

//////////////////////////////////

void setup()
{
  initializeBoard();  
  initializeGPS();
  initializeGPRS();
  initializeDevice();
}

void loop()
{
  curTime = millis();
  
  if (emitting == 1) {
    if (curTime > (newTime + interval))
    {
      emitGPS();
      
      newTime = curTime; 
    };
  };
  
  Device.Process(); 
}

