import processing.serial.*;
import cc.arduino.*;
import controlP5.*;
final int X = 50, r = 5;
int connect = -1, x;
float timing = 0;
boolean val, mPressed, textBoxFlag = false, timer_flag = false;
String[] setTimeFields = {"1", "2", "3", "4", "5", "6"};
Arduino arduino;
PFont font;
ControlP5 cp5;


void setup() 
{
  size(520, 170);
  surface.setResizable(true);
  font = createFont("arial", 20);
  cp5 = new ControlP5(this);
  
}


void drawActualTime()
{
  String stroke = "Время устройства: ";
  arduino.analogWrite(9, 0);
  stroke += arduino.analogRead(1) + ":";
  stroke += arduino.analogRead(2) + ":";
  stroke += arduino.analogRead(3);
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
  println("gg");
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
      timing = arduino.analogRead(4);
      
      if(timing > 0)
      {
        println("fds" + timing/10);
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
    }
    //text(arduino.analogRead(4), 10, 90);
    drawSetTime();
    drawActualTime();
  }
  else
  {
    timing = 0;
    timer_flag = true;
  }
  
}
