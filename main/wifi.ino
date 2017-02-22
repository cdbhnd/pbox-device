///// VARIABLES /////
#define WIFI_AP "cbd wireless 2G"
#define WIFI_PASSWORD "kodbihajnd"
#define WIFI_AUTH LWIFI_WPA
/////////////////////
void initializeWIFI()
{
  Serial.println("starting");
  LWiFi.begin();   
  Serial.print("Connecting to WiFi AP: ");
  Serial.println(WIFI_AP);
  while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD)))
    delay(1000);
  Serial.println("connected");
};



  


