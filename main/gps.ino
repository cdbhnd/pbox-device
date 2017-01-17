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

void initializeLocation(struct Locations loc) 
{
  loc.satellite_status = 0;
  loc.latitude = 0.0;
  loc.longitude = 0.0;
  
  sprintf(buff, " satellite_status: %d\n latitude: %f\n longitude: %f\n", loc.satellite_status, loc.latitude, loc.longitude);
  Serial.println(buff);
};

struct Locations parseGPGGA(const char* GPGGAstr)
{
  int tmp;
  struct Locations loc;
  if(GPGGAstr[0] == '$')
  {
    tmp = getComma(7, GPGGAstr);
    loc.satellite_status = getIntNumber(&GPGGAstr[tmp]);    

    if(loc.satellite_status != 0)
    {
      tmp = getComma(2, GPGGAstr);
      loc.latitude = getDoubleNumber(&GPGGAstr[tmp]) * 0.01;
      
      tmp = getComma(4, GPGGAstr);
      loc.longitude = getDoubleNumber(&GPGGAstr[tmp]) * 0.01;
      
      sprintf(buff, "latitude = %f\nlongitude = %f", loc.latitude, loc.longitude);
      Serial.println(buff);

      return loc;
    }
    else 
    {
      Serial.printf("Satellites not yet fixed!\n");

      return loc;
    };
  }
  else
  {
    Serial.println("Not get data"); 
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////


void initializeGps()                                     // this method is first to be called during setup process 
{
  Serial.println("\n====== GPS initialization ======");
  
  initializeLocation(tmpLocation);
  
  LGPS.powerOn();
  
  Serial.println("LGPS Power on, and waiting ..."); 
  
  delay(3000);
}

struct Locations getGpsData()                           // use this method to acquire gps cordinates after initialization has been made
{
  LGPS.getData(&info);
  
  Serial.println("RAW data: ");
  Serial.println((char*)info.GPGGA); 
  
  tmpLocation = parseGPGGA((const char*)info.GPGGA);
  
  if(tmpLocation.satellite_status == 0)
  {   
       return getGpsData();
  }
  else
  {
    return tmpLocation;
  };
}
