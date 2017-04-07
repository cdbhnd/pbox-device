///// VARIABLES /////
char apn[] = "NET";
char username[] = "telenor";
char password[] = "gprs";

/////////////////////
void initializeGPRS()
{
  while(!LGPRS.attachGPRS(apn,username,password))
  {
    delay(1000);
  }
};
