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
  batteryLevel = LBattery.level();
  batteryCharging = LBattery.isCharging();

  if(batteryLevel == 0 && batteryCharging == 0)
  {
    emitting = 0;
    Device.Send(String("false"), actId);
    Device.Send(String("Low batter, connect charger."), batId);
  };
};

void initializeBoard() {
  emitting = 0;
  
//  Serial.begin(115200);
//  while(!Serial);
};

