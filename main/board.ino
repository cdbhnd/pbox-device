///// VARIABLES /////
char batt[256];

/////////////////////
void getBattery()
{
  sprintf(batt,"%d%%\0", LBattery.level());

};

void emitBattery()
{
  getBattery();
  
  Device.Send(String(batt), batId);

};

void checkBatteryStatus()
{
  batteryLevel1 = LBattery.level();
  batteryCharging1 = LBattery.isCharging();

  if(batteryLevel1 == 0 && batteryCharging1 == 0)
  {
    emitting = 0;
    Device.Send(String("false"), actId);
    Device.Send(String("Connect charger"), batId);
  };
};

void initializeBoard() {
  emitting = 0;
  
//  Serial.begin(115200);
//  while(!Serial);
};

