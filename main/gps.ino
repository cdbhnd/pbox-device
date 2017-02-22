///// VARIABLES /////
gpsSentenceInfoStruct info;
char buff[256];
String gpsCoords;
double latitude = 0.00;
double longitude = 0.00;
float altitude = 0.00;
float dop = 100.00;
float geoid = 0.00;
float k_speed = 0.00, m_speed = 0.00;
float track_angle = 0.00;
int fix = 0;
int hour = 0, minute = 0, second = 0;
int sat_num = 0;
int day = 0, month = 0, year = 0;
String time_format = "00:00:00", date_format = "00:00:0000";
String lat_format = "0.00000", lon_format = "0.00000";
char file[15] = "";

///// HELPER FUNCTIONS /////
float convert(String str, boolean dir)
{
  double mm, dd;
  int point = str.indexOf('.');
  dd = str.substring(0, (point - 2)).toFloat();
  mm = str.substring(point - 2).toFloat() / 60.00;
  return (dir ? -1 : 1) * (dd + mm);
}

////////////////////////////
int getData(gpsSentenceInfoStruct* info)
{
//   Serial.println("Collecting GPS data.");
//   Serial.println((char*)info->GPGGA);
  if (info->GPGGA[0] == '$')
  {
    // Serial.print("Parsing GGA data....");
    String str = (char*)(info->GPGGA);
    str = str.substring(str.indexOf(',') + 1);
    hour = str.substring(0, 2).toInt() + 1;
    minute = str.substring(2, 4).toInt();
    second = str.substring(4, 6).toInt();
    time_format = "";
    time_format += hour;
    time_format += ":";
    time_format += minute;
    time_format += ":";
    time_format += second;
    str = str.substring(str.indexOf(',') + 1);
    latitude = convert(str.substring(0, str.indexOf(',')), str.charAt(str.indexOf(',') + 1) == 'S');
    int val = latitude * 1000000;
    String s = String(val);
    lat_format = s.substring(0, (abs(latitude) < 100) ? 2 : 3);
    lat_format += '.';
    lat_format += s.substring((abs(latitude) < 100) ? 2 : 3);
    str = str.substring(str.indexOf(',') + 3);
    longitude = convert(str.substring(0, str.indexOf(',')), str.charAt(str.indexOf(',') + 1) == 'W');
    val = longitude * 1000000;
    s = String(val);
    lon_format = s.substring(0, (abs(longitude) < 100) ? 2 : 3);
    lon_format += '.';
    lon_format += s.substring((abs(longitude) < 100) ? 2 : 3);

    str = str.substring(str.indexOf(',') + 3);
    fix = str.charAt(0) - 48;
    str = str.substring(2);
    sat_num = str.substring(0, 2).toInt();
    str = str.substring(3);
    dop = str.substring(0, str.indexOf(',')).toFloat();
    str = str.substring(str.indexOf(',') + 1);
    altitude = str.substring(0, str.indexOf(',')).toFloat();
    str = str.substring(str.indexOf(',') + 3);
    geoid = str.substring(0, str.indexOf(',')).toFloat();
    // Serial.println("done.");
    
    if (info->GPRMC[0] == '$')
    {
    //   Serial.print("Parsing RMC data....");
      str = (char*)(info->GPRMC);
      int comma = 0;
      for (int i = 0; i < 60; ++i)
      {
        if (info->GPRMC[i] == ',')
        {
          comma++;
          if (comma == 7)
          {
            comma = i + 1;
            break;
          }
        }
      }

      str = str.substring(comma);
      k_speed = str.substring(0, str.indexOf(',')).toFloat();
      m_speed = k_speed * 0.514;
      str = str.substring(str.indexOf(',') + 1);
      track_angle = str.substring(0, str.indexOf(',')).toFloat();
      str = str.substring(str.indexOf(',') + 1);
      day = str.substring(0, 2).toInt();
      month = str.substring(2, 4).toInt();
      year = str.substring(4, 6).toInt();
      date_format = "20";
      date_format += year;
      date_format += "-";
      date_format += month;
      date_format += "-";
      date_format += day;
    //   Serial.println("done.");
      return sat_num;
    }
  }
  else
  {
    // Serial.println("No GGA data");
  }
  return 0;
}

void getGpsData()
{
  LGPS.getData(&info);
  
  Serial.println("RAW data: ");
  Serial.println((char*)info.GPGGA); 
  
  sat_num = getData(&info);

  if(sat_num > 3)
  {
    setLedBar();

    gpsCoords += lat_format;
    gpsCoords += ",";
    gpsCoords += lon_format;
    
    fixed = 1;
    setLoopInterval(emittingDelay);
  }
  else
  {
    bar.setBits(random(1024));
      
    fixed = 0;
    setLoopInterval(fixingDelay);
  };
}

void emitGPS() 
{
  getGpsData();
  
  if (fixed == 1)
  {
    Device.Send(String(gpsCoords), gpsId);
    gpsCoords = "";
  };
};

void initializeGPS()
{  
  setLoopInterval(1000);
  fixed = 0;
  
  LGPS.powerOn();
  
//  Serial.println("GPS Power on, and waiting ..."); 
  
  delay(3000);
}

