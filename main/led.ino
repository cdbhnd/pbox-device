void turnOffLedBar()
{
  bar.setBits(0x0);
};

void setLedBar()
{
  if (LBattery.level() == 100)
  {
    bar.setBits(0x3ff);
  }
  else if(LBattery.level() == 66)
  {
    bar.setBits(0b000000000111111);
  }
  else if(LBattery.level() == 33)
  {
    bar.setBits(0b000000000000111);
  }
  else
  {
    bar.setBits(0x1);  
  };
};

void initializeLED()
{
  bar.begin();
};
