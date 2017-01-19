void initializeBoard() {
  emitting = 0;
  fixed = 0;
  
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

void initializeAccelerometer()
{
  adxl.powerOn();
  
  //set activity/ inactivity thresholds (0-255)
  adxl.setActivityThreshold(75); //62.5mg per increment
  adxl.setInactivityThreshold(75); //62.5mg per increment
  adxl.setTimeInactivity(10); // how many seconds of no activity is inactive?
 
  //look of activity movement on this axes - 1 == on; 0 == off 
  adxl.setActivityX(1);
  adxl.setActivityY(1);
  adxl.setActivityZ(1);
 
  //look of inactivity movement on this axes - 1 == on; 0 == off
  adxl.setInactivityX(1);
  adxl.setInactivityY(1);
  adxl.setInactivityZ(1);
 
  //look of tap movement on this axes - 1 == on; 0 == off
  adxl.setTapDetectionOnX(0);
  adxl.setTapDetectionOnY(0);
  adxl.setTapDetectionOnZ(1);
 
  //set values for what is a tap, and what is a double tap (0-255)
  adxl.setTapThreshold(50); //62.5mg per increment
  adxl.setTapDuration(15); //625us per increment
  adxl.setDoubleTapLatency(80); //1.25ms per increment
  adxl.setDoubleTapWindow(200); //1.25ms per increment
 
  //set values for what is considered freefall (0-255)
  adxl.setFreeFallThreshold(7); //(5 - 9) recommended - 62.5mg per increment
  adxl.setFreeFallDuration(45); //(20 - 70) recommended - 5ms per increment
 
  //setting all interrupts to take place on int pin 1
  //I had issues with int pin 2, was unable to reset it
  adxl.setInterruptMapping( ADXL345_INT_SINGLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_DOUBLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_FREE_FALL_BIT,    ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_ACTIVITY_BIT,     ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_INACTIVITY_BIT,   ADXL345_INT1_PIN );
 
  //register interrupt actions - 1 == on; 0 == off  
  adxl.setInterrupt( ADXL345_INT_SINGLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_DOUBLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_FREE_FALL_BIT,  1);
  adxl.setInterrupt( ADXL345_INT_ACTIVITY_BIT,   1);
  adxl.setInterrupt( ADXL345_INT_INACTIVITY_BIT, 1);
};

void initializeDevice()
{
  while(!Device.Connect(&c, httpServer))
  {
//    Serial.println("retrying ...");
  };
    
  Device.AddAsset(id, "gps", "Global Positioning System", false, "string");
  Device.AddAsset(actId, "switch", "turning on and off emitting", true, "boolean");
  Device.AddAsset(accId, "acceleromaeter", "acceleration", false, "string");
  
  while(!Device.Subscribe(pubSub))
  {
//    Serial.println("retrying ...");
  };
  
  Device.Send(String("false"), actId);

//  Serial.println("ACTIVE!!!");
};
