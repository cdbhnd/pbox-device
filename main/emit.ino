void emitGPS() 
{
  getGpsData();
  
  if (fixed == 1){
    Device.Send(String(gpsCoords), id);
  };
  
};

void emitTemperature()
{
  
  getTemperature();
  
  Device.Send(String(temperatureData), dhtId);
};


void emitBattery()
{
  getBattery();
  
  Device.Send(String(battery), batId);
};


