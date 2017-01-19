//////////////////////////// HELPER VARIABLES //////////////////////////////////
char buff[256];
//////////////////////////// HELPER FUNCTIONS //////////////////////////////////
static unsigned char getComma(unsigned char num,const char *str)
{
  unsigned char i,j = 0;
  int len=strlen(str);
  for(i = 0;i < len;i ++)
  {
     if(str[i] == ',')
      j++;
     
     if(j == num)
      return i + 1; 
  }
  return 0; 
}

static double getDoubleNumber(const char *s)
{
  char buf[10];
  unsigned char i;
  double rev;
  
  i=getComma(1, s);
  i = i - 1;
  strncpy(buf, s, i);
  buf[i] = 0;
  rev=atof(buf);
  
  return rev; 
}

static double getIntNumber(const char *s)
{
  char buf[10];
  unsigned char i;
  double rev;
  
  i=getComma(1, s);
  i = i - 1;
  strncpy(buf, s, i);
  buf[i] = 0;
  rev=atoi(buf);
  
  return rev; 
}

void initializeLocation() 
{
  satellite_status = 0;
  latitude = 0.0;
  longitude = 0.0;
};

void setLoopInterval(int sec) 
{
  interval = sec;
};

void parseGPGGA(const char* GPGGAstr)
{
  int tmp;
  if(GPGGAstr[0] == '$')
  {
    tmp = getComma(7, GPGGAstr);
    satellite_status = getIntNumber(&GPGGAstr[tmp]);    

    if(satellite_status != 0)
    {
      bar.setBits(0x3ff);
      
      tmp = getComma(2, GPGGAstr);
      latitude = getDoubleNumber(&GPGGAstr[tmp]) * 0.01;
      tmp = getComma(4, GPGGAstr);
      longitude = getDoubleNumber(&GPGGAstr[tmp]) * 0.01;
            
      sprintf(buff, "%f,%f\0", latitude, longitude);

      gpsCoords = buff;
    
      fixed = 1;
      setLoopInterval(emittingDelay);
    }
    else 
    {
      bar.setBits(random(1024));
      
      fixed = 0;
      setLoopInterval(fixingDelay);

//      Serial.printf("Satellites not yet fixed!\n");
    };
  }
  else
  {
//    Serial.println("Not getting data"); 
  }
}

void getGpsData()                           // use this method to acquire gps cordinates after initialization has been made
{
  LGPS.getData(&info);
  
//  Serial.println("RAW data: ");
//  Serial.println((char*)info.GPGGA); 
  
  parseGPGGA((const char*)info.GPGGA);
}

void getTemperature()
{
  dht.readHT(&temperature, &humidity);

  if (isnan(temperature) || isnan(humidity)) 
    {
      temperatureData = "0.0";
//        Serial.println("Failed to read from DHT");
    } 
    else 
    {
        sprintf(buff, "%4.2fC,%4.2f%%\0", temperature, humidity);

        temperatureData = buff;
    }
};

void getBattery()
{
  sprintf(battery,"%d%%\0", LBattery.level());
};
