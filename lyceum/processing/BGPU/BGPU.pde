import processing.serial.*;
//import cc.arduino.*;
import controlP5.*;
import java.util.Date;
final int X = 50, r = 5;
int connect = -1, x;
float timing = 0;
long lastS = millis();
boolean val, mPressed, textBoxFlag = false, timer_flag = false;
String[] setTimeFields = {"1", "2", "3", "4", "5", "6"};
int[] data = {0, 0, 0, 0, 0, 0, 0, 0};
byte r_pos = 0;
PFont font;
ControlP5 cp5;
Serial arduino;       
void setup() 
{
  printArray(Serial.list());
  size(520, 170);
  surface.setResizable(true);
  font = createFont("arial", 20);
  cp5 = new ControlP5(this);
  
}


void drawActualTime()
{
  String stroke = "Время устройства: ";
  stroke += data[2] + ":";
  stroke += data[1] + ":";
  stroke += data[0];
  stroke += "  timing: " + data[7] / 10.0;
  textSize(18);
  text(stroke, 10, 20);
  stroke = "Время компьютера: ";
  stroke += hour() + ":";
  stroke += minute() + ":";
  stroke += second();
  text(stroke, 10, 45);
}
void slider(float get_timing)
{
  timing = int(get_timing * 10);
}
void readData(int num)
{
  if(millis() - lastS >= 80)
  {
      r_pos = 0;
  }
  for(int j = 0; j < num; j++)
  {
    int r_data = unsignedByte((byte)arduino.read());
    data[r_pos] = r_data;
    if(r_pos == 7)
      r_pos = 0;
     else
       r_pos ++;
  }
  if(num != 0)
  {
    lastS = millis();
  }
  
}
void draw() 
{
  stroke(on);
  background(white);
  textFont(font);
  drawButtons();
  if (val)
  {
    if(timer_flag)
    {
      
      while(data[7] == 0)
      {
        readData(arduino.available());
        delay(10);
      }
      timing = data[7];
        float test = timing/10;
        cp5.addSlider("slider")
        .setRange(0.5, 25.5)
        .setNumberOfTickMarks(51)
        .setValue(test)
        .setPosition(210,55)
        .setSize(200,40)
        .setColorValue(black)
        .setFont(font)
        .setColorActive(mouse_on)
        .setColorForeground(color(205, 205, 205))
        .setColorBackground(on)
        ;
        timer_flag = false;
      
    }
    readData(arduino.available());
    drawSetTime();
    drawActualTime();
  }
  else
  {
    timing = 0;
    timer_flag = true;
  }
  
}
