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

void initializeBoard() {
  emitting = 0;
  
//  Serial.begin(115200);
//  while(!Serial);
};
