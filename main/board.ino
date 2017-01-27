///// VARIABLES /////
char batt[256];

/////////////////////
void getBattery()
{
  sprintf(batt,"%d%%,%d\0", LBattery.level(), LBattery.isCharging());
};

void emitBattery()
{
  getBattery();
  
  Device.Send(String(batt), batId);

};

void checkBatteryStatus()
{
  batLevel = LBattery.level();
  batCharging = LBattery.isCharging();

  sprintf(batt,"%d%%,%d\0", LBattery.level(), LBattery.isCharging());  

  if(batLevel == 0 && batCharging == 0)
  {
    emitting = 0;
    Device.Send(String("false"), actId);
    Device.Send(String(batt), batId);
  };
};

void initializeBoard() {
  emitting = 0;
  
//  Serial.begin(115200);
//  while(!Serial);
};

