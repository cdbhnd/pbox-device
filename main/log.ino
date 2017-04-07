//uncomment the storage you want to use
#define Drv LFlash          // use Internal 10M Flash
// #define Drv LSD           // use SD card

void logData()
{
  String logg = "";
  logg += time_format;
  logg += "|";
  logg += batt;
  logg += "|";
  logg += lat_format;
  logg += ",";
  logg += lon_format;
  logg += "|";
  logg += sat_num;
  logg += "|";
  logg += temperatureData;
  logg += "|";
  logg += accelerationData;
  
  String file_name = date_format;
  file_name += ".txt";
  file_name.toCharArray(file, 14);
  LFile dataFile = Drv.open(file, FILE_WRITE);
  if (dataFile)
  {
    dataFile.println(logg);
    dataFile.close();
  }
  else
  {
  };
};

void initializeLog()
{
  logging = 0;
  pinMode(10, OUTPUT); //needed for SD card
  if(!Drv.begin())
  {
    while(true);
  }
};
