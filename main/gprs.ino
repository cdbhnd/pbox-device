///// VARIABLES /////
char apn[] = "NET";
char username[] = "telenor";
char password[] = "gprs";

/////////////////////
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
