void initializeBoard() {
  emitting = 0;
  fixed = 0;

  bar.begin();
  
//  Serial.begin(115200);
//  while(!Serial);
};

void initializeGPS()                                     // this method is first to be called during setup process 
{  
  initializeLocation();
  
  setLoopInterval(1000);
  
  LGPS.powerOn();
  
//  Serial.println("GPS Power on, and waiting ..."); 
  
  delay(3000);
}

void initializeGPRS()
{
//  Serial.println("GPRS connecting ...");

  while(!LGPRS.attachGPRS(apn,username,password))
  {
//    Serial.println("connecting ...");
    delay(1000);
  }
  
//  Serial.println("GPRS connected");
};

void initializeDHT()
{
  dht.begin();
};

void initializeDevice()
{
  while(!Device.Connect(&c, httpServer))
  {
//    Serial.println("retrying ...");
  };
    
  Device.AddAsset(id, "gps", "Global Positioning System", false, "string");
  Device.AddAsset(actId, "switch", "turning on and off emitting", true, "boolean");
  Device.AddAsset(dhtId, "dht", "temperature and humidity sensor", false, "string");
  Device.AddAsset(batId, "battery", "battery status", false, "string");
  
  while(!Device.Subscribe(pubSub))
  {
//    Serial.println("retrying ...");
  };
  
  Device.Send(String("false"), actId);

//  Serial.println("ACTIVE!!!");
};
