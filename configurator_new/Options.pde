color on = color(234, 230, 230);
color mouse_on = color(190, 190, 190);
color mouse_yellow = color(240, 240, 0);
color black = color(0, 0, 0);
color white = color(255, 255, 255);
color red = color(255, 51, 51);
color blue = color(51, 51, 255);
color purple = color(153, 0, 153);
color green = color(51, 255, 51);
color yellow = color(255, 255, 51);
color brown = color(153, 76, 0);
void drawSetTime()
{
  
  fill(black);
  textSize(15);
  text("Длительность звонка (Сек.):",10, 90);
  textSize(30);
}

void mousePressed()
{
  if(val)
  {
    fill(black);
    if (mouseX >= 370 + X && mouseX <= 440 + X && mouseY >= 10 && mouseY <= 35)
    {
      fill(red);
      rect(370 + X, 10, 70, 25, r);
      arduino.write(second());
      arduino.write(minute());
      arduino.write(hour());
      arduino.write(day());
      arduino.write(month());
      arduino.write(year() - 2000);
      arduino.write(new Date().getDay());
      arduino.write((int)timing);
      
    }
    else if (mouseX >= 405 + X && mouseX <= 440 + X && mouseY >= 105 && mouseY <= 140)
    {
      fill(yellow);
      rect(405 + X, 105, 35, 35, r);
      connect = -1;
      val = false;
      cp5.remove("slider");
      textBoxFlag = false;
      arduino.stop();
    }
  }
  else
  {
    for(int i = 0; i < Serial.list().length; i ++)
    {
      if (mouseX >= 30 && mouseX <= 90 && mouseY >= i * 35 && mouseY <= 25 + i * 35)
      {
        fill(mouse_on);
        rect(20, i * 35, 70, 25, r);
        fill(black);
        textSize(18);
        text(Serial.list()[i], 25, 20 + i * 35);
        connect = i;
      }
    }
  }
}
void drawButtons()
{
  if(val)
  {
    fill(red);
    rect(405 + X, 105, 35, 35, r);
    fill(yellow);
    rect(370 + X, 10, 70, 25, r);
    if (mouseX >= 370 + X && mouseX <= 440 + X && mouseY >= 10 && mouseY <= 35)
    {
      fill(mouse_yellow);
      rect(370 + X, 10, 70, 25, r);
    }
    fill(black);
    textSize(16);
    text("Записать", 370 + X, 30);
    textSize(30);
    text("X", 413 + X, 134);
  }
  else
  {
    for(int i = 0; i < Serial.list().length; i++)
    {
      
      fill(on);
      rect(20, i * 35, 70, 25, r);
      fill(black);
      textSize(18);
      text(Serial.list()[i], 25, 20 + i * 35);
    }
    if(connect != -1)
    {
      try
        {

          arduino = new Serial(this, Serial.list()[connect], 2000000);
          val = true;
          delay(2000);
          arduino.write(228);
        }
        catch(java.lang.RuntimeException e)
        {
          println("Port is busy");
        }
      connect = -1;
    }
  }
}
int unsignedByte(byte val)
{
  return (int)(val < 0 ? 256 + val: val); 
}
