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
    setLoopInterval(offDelay);
  }
  if(idOut != NULL)                //also let the iot platform know that the operation was succesful: give it some feedback. This also allows the iot to update the GUI's correctly & run scenarios.
    Device.Send(msgString, *idOut);    
}
