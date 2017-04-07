///// VARIABLES /////
#define WIFI_AP "cbd wireless 2G"
#define WIFI_PASSWORD "kodbihajnd"
#define WIFI_AUTH LWIFI_WPA
/////////////////////
void initializeWIFI()
{
  LWiFi.begin();   
  while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD)))
  delay(1000);
};



  


