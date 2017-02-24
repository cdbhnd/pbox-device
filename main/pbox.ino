void createBox(String boxName,char* deviceId) {
  
  Serial.println("Connecting to PBOX server");
  
  while (0 == c.connect(PBOX_SERVER, PBOX_SERVER_PORT))
  {
    Serial.println("Re-Connecting to PBOX server");
    delay(1000);
  } 
 
  String data = "{\"code\": \""+ boxName +"\", \"deviceId\": \""+ deviceId +"\"}";
  //String thisLength = String(data.length());

  Serial.println("Creating BOX in system ...");
  
//  // Build HTTP POST request
//  c.print("POST /v1.0/boxes");
//  c.println(" HTTP/1.1");
//  c.print("Host: ");
//  c.println(PBOX_SERVER);
//  c.println("Authorization: Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJhdXRoVXNlcklkIjoiNTg5ZDhlZDE2ZjExZDQwMDA0NmQzZTUzIn0.h58hw0TivKLIaWlvG8cUlxxezduNu-QZG8XbgX4wjwY");
//  c.println("Content-Type: application/json");
// // c.println("Content-Length: " + thisLength);
//  c.println("Cache-Control: no-cache");
//  c.print("\n" + data);
//  c.print(char(26));



    // send HTTP request, ends with 2 CR/LF
    Serial.print("POST /v1.0/boxes");
    Serial.println(" HTTP/1.1");
    Serial.print("Host: ");
    Serial.println(PBOX_SERVER);
    Serial.println("Authorization: Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJhdXRoVXNlcklkIjoiNTg5ZDhlZDE2ZjExZDQwMDA0NmQzZTUzIn0.h58hw0TivKLIaWlvG8cUlxxezduNu-QZG8XbgX4wjwY");
    Serial.println("Content-Type: application/json");
    Serial.print("\n" + data);
    Serial.print(char(26));
    Serial.println("Connection: close");
    Serial.println();
    c.print("POST /v1.0/boxes");
    c.println(" HTTP/1.1");
    c.print("Host: ");
    c.println(PBOX_SERVER);
    c.println("Authorization: Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJhdXRoVXNlcklkIjoiNTg5ZDhlZDE2ZjExZDQwMDA0NmQzZTUzIn0.h58hw0TivKLIaWlvG8cUlxxezduNu-QZG8XbgX4wjwY");
    c.println("Content-Type: application/json");
    c.print("\n" + data);
    c.print(char(26));
    c.println("Connection: close");
    c.println();

    while (c){
      Serial.print((char)c.read());
    }
  
////    // waiting for server response
//    Serial.println("waiting HTTP response:");
//    while (!c.available())
//    {
//      delay(100);
//    }
//
//    boolean disconnectedMsg = false;
//
//    while (c)
//    {
//      int v = c.read();
//      if (v != -1)
//      {
//        Serial.print((char)v);
//      }
//      else
//      {
//        Serial.println("no more content, disconnect");
//        c.stop();
//        while (1)
//        {
//          delay(1);
//        }
//      }
//    }
//
//    if (!disconnectedMsg)
//    {
//      Serial.println("disconnected by server");
//      disconnectedMsg = true;
//    }
//    delay(500);
}
