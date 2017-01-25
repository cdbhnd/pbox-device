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
//  Serial.println(logg);
  
  String file_name = date_format;
  file_name += ".txt";
  file_name.toCharArray(file, 14);
//  Serial.println(file);
  LFile dataFile = Drv.open(file, FILE_WRITE);
  if (dataFile)
  {
    dataFile.println(logg);
    dataFile.close();
//    Serial.println("File written.");
  }
  else
  {
//    Serial.println("Error opening file.");
  };
};

void initializeLog()
{
  logging = 0;
//  Serial.print("Initializing memory...");
  pinMode(10, OUTPUT); //needed for SD card
  if(!Drv.begin())
  {
//    Serial.println("Error initalizing memory.");  
    while(true);
  }
//  Serial.println("OK.");
};
