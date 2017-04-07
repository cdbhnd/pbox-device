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

  if(batLevel == 0 && batCharging == 0)
  {
    if(emitting == 1) 
    {
      emitting = 0;
      Device.Send(String("false"), actId);
      sprintf(batt,"%d%%,%d\0", LBattery.level(), LBattery.isCharging());  
      Device.Send(String(batt), batId);
    };
  };
};

void initializeBoard() {
  emitting = 0;
};

