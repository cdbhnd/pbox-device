///// VARIABLES /////
ADXL345 adxl;
int accelerometerThreshold = 1;
char acce[256];
char vib[5];
int x;
int y;
int z;
double accelerometerAcceleration[3];
double prevAccelerometerAcceleration[3];
bool vibrating = false;
String accelerationData;
String vibrationData;

/////////////////////
void getAcceleration()
{
  adxl.getAcceleration(accelerometerAcceleration);

  double difference = 0;
  int numberOfAxis = 0;

  if (accelerometerAcceleration[0] >= prevAccelerometerAcceleration[0]) {
    difference = accelerometerAcceleration[0] - prevAccelerometerAcceleration[0];
  } else {
    difference = prevAccelerometerAcceleration[0] - accelerometerAcceleration[0];
  }
  if (difference > accelerometerThreshold) {
    prevAccelerometerAcceleration[0] = accelerometerAcceleration[0];
    numberOfAxis++;
  }

  if (accelerometerAcceleration[1] >= prevAccelerometerAcceleration[1]) {
    difference = accelerometerAcceleration[1] - prevAccelerometerAcceleration[1];
  } else {
    difference = prevAccelerometerAcceleration[1] - accelerometerAcceleration[1];
  }
  if (difference > accelerometerThreshold) {
    prevAccelerometerAcceleration[1] = accelerometerAcceleration[1];
    numberOfAxis++;
  }

  if (accelerometerAcceleration[2] >= prevAccelerometerAcceleration[2]) {
    difference = accelerometerAcceleration[2] - prevAccelerometerAcceleration[2];
  } else {
    difference = prevAccelerometerAcceleration[2] - accelerometerAcceleration[2];
  }
  if (difference > accelerometerThreshold) {
    prevAccelerometerAcceleration[2] = accelerometerAcceleration[2];
    numberOfAxis++;
  }

  if (numberOfAxis > 2) {
    vibrating = true;
  }

  sprintf(acce, "%4.2f,%4.2f,%4.2f\0", prevAccelerometerAcceleration[0], prevAccelerometerAcceleration[1], prevAccelerometerAcceleration[2]);
  sprintf(vib, "%d", vibrating);
//  Serial.println("acce");
//  Serial.println(acce);
//  Serial.println("vibrating");
//  Serial.printf("%d", vibrating);
//  Serial.println("diference");
//  Serial.printf("%d", difference);

  accelerationData = acce;
  vibrationData = vib;  
}

void emitAcceleration()
{
  getAcceleration();

  Device.Send(String(accelerationData), accId);
  Device.Send(String(vibrationData), vibId);
  vibrating = false;
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

}
