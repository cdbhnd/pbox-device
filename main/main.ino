#include <LGPS.h>
#include <LGPRS.h>
#include <LGPRSClient.h>
#include <LGPRSServer.h>
#include <LGPRSUdp.h>
#include <PubSubClient.h>
#include <ATT_IOT.h>
#include <Grove_LED_Bar.h>
#include <DHT.h>
#include <LBattery.h>
#include <Wire.h>
#include <ADXL345.h>

#define httpServer "api.AllThingsTalk.io"                  
#define mqttServer "broker.smartliving.io"

///////// LED_BAR /////////
Grove_LED_Bar bar(9, 8, 0);

///////// DHT /////////
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
static float temperature = 0.0;
static float humidity = 0.0;
String temperatureData;

///////// BATTERY /////////
char battery[256];

///////// ACCELEROMETER /////////
ADXL345 adxl;
int x;
int y;
int z;
double xyz[3];
double ax,ay,az;
String accelerationData;

///////// DEVICE_VARIABLES /////////
char deviceId[] = "tIfnBDpabrH4K6aF5l2RCAnv";
char clientId[] = "ognjent_EjGwg4VW";
char clientKey[] = "GsqcBmey";
ATTDevice Device(deviceId, clientId, clientKey);

///////// ASSETS_ID'S /////////
int id = 2;           // GPS sensor
int actId = 3;        // Switch actuator
int dhtId = 4;        // DHT sensor
int batId = 5;        // Battery
int accId = 6;         // Accelerometer 


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
  initializeDHT();
  initializeAccelerometer();
  initializeDevice();
}

void loop()
{
  curTime = millis();
  
  if (emitting == 1) {
    if (curTime > (newTime + interval))
    {
      emitGPS();
      emitTemperature();
      emitBattery();
      emitAcceleration();
      
      newTime = curTime; 
    };
  } else {
    bar.setBits(0x0);
  };
  
  Device.Process(); 
}

