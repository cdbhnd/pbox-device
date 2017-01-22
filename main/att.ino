// Callback function: handles messages that were sent from the iot platform to this device.
void callback(char* topic, byte* payload, unsigned int length) 
{ 
  String msgString; 
  {                                                            //put this in a sub block, so any unused memory can be freed as soon as possible, required to save mem while sending data
    char message_buff[length + 1];                        //need to copy over the payload so that we can add a /0 terminator, this can then be wrapped inside a string for easy manipulation.
    strncpy(message_buff, (char*)payload, length);        //copy over the data
    message_buff[length] = '\0';                      //make certain that it ends with a null           
          
    msgString = String(message_buff);
    msgString.toLowerCase();                  //to make certain that our comparison later on works ok (it could be that a 'True' or 'False' was sent)
  }
  int* idOut = NULL;
  int* idOut2 = NULL;
  {                                                       //put this in a sub block, so any unused memory can be freed as soon as possible, required to save mem while sending data
    int pinNr = Device.GetPinNr(topic, strlen(topic));
    
//    Serial.print("Payload: ");                            //show some debugging.
//    Serial.println(msgString);
//    Serial.print("topic: ");
//    Serial.println(topic);

    if (pinNr == actId)
    {
      if (msgString == "false") {
//        Serial.println("Emitting OFF");  
        idOut = &actId;
        emitting = 0;                             
      }
      else if (msgString == "true") {
//        Serial.println("Emitting ON");
        idOut = &actId;
        emitting = 1;
      }
    }
    if (pinNr == logId)
    {
      if (msgString == "false") {
//        Serial.println("Log OFF");  
        idOut2 = &logId;
        logging = 0;                             
      }
      else if (msgString == "true") {
//        Serial.println("Log ON");
        idOut2 = &logId;
        logging = 1;
      }
    }
    setLoopInterval(offDelay);
  }
  if(idOut != NULL)                //also let the iot platform know that the operation was succesful: give it some feedback. This also allows the iot to update the GUI's correctly & run scenarios.
    Device.Send(msgString, *idOut); 
    Device.Send(msgString, *idOut2);   
}

void initializeATT()
{
  while(!Device.Connect(&c, httpServer))
  {
//    Serial.println("retrying ...");
  };
    
  Device.AddAsset(gpsId, "gps", "Global Positioning System", false, "string");
  Device.AddAsset(actId, "switch", "turning on and off emitting", true, "boolean");
  Device.AddAsset(dhtId, "dht", "temperature and humidity sensor", false, "string");
  Device.AddAsset(batId, "battery", "battery status", false, "string");
  Device.AddAsset(accId, "acceleromaeter", "acceleration", false, "string");
  Device.AddAsset(logId, "log", "logging data", true, "boolean");

  while(!Device.Subscribe(pubSub))
  {
//    Serial.println("retrying ...");
  };
  
  Device.Send(String("false"), actId);
  Device.Send(String("false"), logId);
//  Serial.println("ACTIVE!!!");
};
