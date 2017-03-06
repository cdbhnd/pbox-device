///// VARIABLES /////
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
static float temperature = 0.0;
static float humidity = 0.0;
String temperatureData;
char temp[256];

/////////////////////

void getTemperature()
{
  dht.readHT(&temperature, &humidity);
    
  if (isnan(temperature) || isnan(humidity)) 
  {
    temperatureData = "0.0C,0.0%%";
//    Serial.println("Failed to read from DHT");
  } 
  else 
  {
    sprintf(temp, "%4.2fC,%4.2f%%\0", temperature, humidity);

    temperatureData = temp;
  }
};

void emitDHT()
{
  getTemperature();
  
  Device.Send(String(temperatureData), dhtId);
};

void initializeDHT()
{
  dht.begin();
};
