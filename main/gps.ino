#include <LGPS.h>

  /* Refer to http://www.gpsinformation.org/dale/nmea.htm#GGA
   * Sample data: $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
   * Where:
   *  GGA          Global Positioning System Fix Data
   *  123519       Fix taken at 12:35:19 UTC
   *  4807.038,N   Latitude 48 deg 07.038' N
   *  01131.000,E  Longitude 11 deg 31.000' E
   *  1            Fix quality: 0 = invalid
   *                            1 = GPS fix (SPS)
   *                            2 = DGPS fix
   *                            3 = PPS fix
   *                            4 = Real Time Kinematic
   *                            5 = Float RTK
   *                            6 = estimated (dead reckoning) (2.3 feature)
   *                            7 = Manual input mode
   *                            8 = Simulation mode
   *  08           Number of satellites being tracked
   *  0.9          Horizontal dilution of position
   *  545.4,M      Altitude, Meters, above mean sea level
   *  46.9,M       Height of geoid (mean sea level) above WGS84
   *                   ellipsoid
   *  (empty field) time in seconds since last DGPS update
   *  (empty field) DGPS station ID number
   *  *47          the checksum data, always begins with *
   */

gpsSentenceInfoStruct info;
char buff[256];
struct Locations location;

void initializeLocation(struct Locations loc) {
  loc.satellite_status = 0;
  loc.latitude = 0.0;
  loc.longitude = 0.0;
  sprintf(buff, " satellite_status: %d\n latitude: %f\n longitude: %f\n", loc.satellite_status, loc.latitude, loc.longitude);
  Serial.println(buff);
};

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

void initializeDevice() {
  Serial.println("\n====== GPS initialization ======");
  initializeLocation(location);
  LGPS.powerOn();
  Serial.println("LGPS Power on, and waiting ..."); 
  delay(3000);
}

struct Locations getGpsData() {
  LGPS.getData(&info);
  Serial.println((char*)info.GPGGA); 
  location = parseGPGGA((const char*)info.GPGGA);
  
  if(location.satellite_status == 0)
  {   
       delay(1000);
       return getGpsData();
  }
  else
  {
    return location;
  };
}
