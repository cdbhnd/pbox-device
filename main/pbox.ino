void createBox(String boxName,char* deviceId) {
  
//Serial.println("Connecting to PBOX server");
  
  while (0 == c.connect(PBOX_SERVER, PBOX_SERVER_PORT))
  {
//    Serial.println("Re-Connecting to PBOX server");
    delay(1000);
  } 
 
  String data = "{\"code\": \""+ boxName +"\", \"deviceId\": \""+ deviceId +"\"}";
  String thisLength = String(data.length());

//  Serial.println("Creating BOX in system ...");

  //Send HTTP POST request
//  Serial.println("POST /v1.0/boxes HTTP/1.1");
//  Serial.println("Host: pbox-test.herokuapp.com");
//  Serial.println("Authorization: Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJhdXRoVXNlcklkIjoiNTg5ZDhlZDE2ZjExZDQwMDA0NmQzZTUzIn0.h58hw0TivKLIaWlvG8cUlxxezduNu-QZG8XbgX4wjwY");
//  Serial.println("User-Agent: Linkit/1.0");
//  Serial.println("Content-Type: application/json");
//  Serial.println("Accept: application/json");
//  Serial.println("Content-Length: " + thisLength);
//  Serial.println("Connection: close");
//  Serial.println();
//  Serial.println(data);
//    
  c.println("POST /v1.0/boxes HTTP/1.1");
  c.println("Host: " PBOX_SERVER);
  c.println("Authorization: Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJhdXRoVXNlcklkIjoiNTg5ZDhlZDE2ZjExZDQwMDA0NmQzZTUzIn0.h58hw0TivKLIaWlvG8cUlxxezduNu-QZG8XbgX4wjwY");
  c.println("User-Agent: Linkit/1.0");
  c.println("Content-Type: application/json");
  c.println("Accept: application/json");
  c.println("Content-Length: " + thisLength);
  c.println("Connection: close");
  c.println();
  c.println(data);

  // waiting for server response
//  Serial.println("waiting HTTP response:");
  while (!c.available())
  {
    delay(100);
  }

  boolean disconnectedMsg = false;

  while (c)
  {
    int v = c.read();
    if (v != -1)
    {
//      Serial.print((char)v);
    }
    else
    {
//      Serial.println("no more content, disconnect");
      c.stop();
      while (1)
      {
        delay(1);
      }
    }
  }

  if (!disconnectedMsg)
  {
//    Serial.println("disconnected by server");
    disconnectedMsg = true;
  }
  delay(500);
}
