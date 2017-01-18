void emitGPS() 
{
  getGpsData();
      
  if (fixed == 1){
    Device.Send(String(gpsCoords), id);
  };
};
