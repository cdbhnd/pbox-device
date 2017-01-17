#include <LGPS.h>
#include <LGPRS.h>
#include <LGPRSClient.h>
#include <LGPRSServer.h>
#include <LGPRSUdp.h>
#include <PubSubClient.h>
#include <ATT_IOT.h>

#define httpServer "api.AllThingsTalk.io"                  
#define mqttServer httpServer 
char deviceId[] = "tIfnBDpabrH4K6aF5l2RCAnv";
char clientId[] = "ognjent_EjGwg4VW";
char clientKey[] = "GsqcBmey";
ATTDevice Device(deviceId, clientId, clientKey);
LGPRSClient c;
int id = 2;

///////////////// GPS_VARIABLES /////////////////////////////////
gpsSentenceInfoStruct info;
struct Locations {
  int satellite_status;
  double latitude;
  double longitude;
};
struct Locations tmpLocation;
struct Locations gpsData;
/////////////////////////////////////////////////////////////////

void callback(char* topic, byte* payload, unsigned int length);

PubSubClient pubSub(mqttServer, 1883, callback, c);

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  
  initializeGps();
  Serial.println("starting");

  while(!LGPRS.attachGPRS("NET","telenor","gprs"))
  {
    Serial.println("connecting");
    delay(1000);
  }
  Serial.println("connected");
  
  while(!Device.Connect(&c, httpServer))
    Serial.println("retrying");
  
  Device.AddAsset(id, "GPS", "Global Positioning System", false, "string");
  
  while(!Device.Subscribe(pubSub))
    Serial.println("retrying"); 
}

unsigned long time;
void loop()
{
  unsigned long curTime = millis();
  if (curTime > (time + 20000))
  {
    gpsData = getGpsData();
    
    int length = 40;
    char* coords = new char[length];
    sprintf(coords, "%f,%f",gpsData.latitude, gpsData.longitude);
    
    Device.Send(String(coords), id);
    
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
  }
  if(idOut != NULL)                //also let the iot platform know that the operation was succesful: give it some feedback. This also allows the iot to update the GUI's correctly & run scenarios.
    Device.Send(msgString, *idOut);    
}

