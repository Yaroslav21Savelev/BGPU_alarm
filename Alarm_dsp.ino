#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define packet_size 7 // starts with 0
#define tt_size 14
#define bBtn 6
#define rBtn 9
#define sBtn 2
#define rPin 3
#define rLED 10
#define gLED 11
#include <iarduino_RTC.h> 
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
iarduino_RTC time(RTC_DS3231); 
unsigned long lastS = 0, stopTimer = 0, lCheck = 0;
bool mode = 0, r_state = 0, beepFlag = 0, bLight = 0, bLastState = 1;
byte timing;
int hour_ac = 0, min_ac = 0, text_pos = 0;
String TEXT = "      bit.ly/bgpu_alarm ";
const byte main_set[tt_size][2] =
{
  {8, 0},
  {8, 40},
  {8, 50},
  {9, 30},
  {9, 40},
  {10, 20},
  {10, 30},
  {11, 10},
  {11, 40},
  {12, 20},
  {12, 30},
  {13, 10},
  {13, 20},
  {14, 00}
};
const byte sat_set[tt_size][2] =
{
  {8, 0},
  {8, 40},
  {8, 50},
  {9, 30},
  {9, 40},
  {10, 20},
  {10, 30},
  {11, 10},
  {11, 20},
  {12, 00},
  {12, 10},
  {12, 50},
  {13, 00},
  {13, 40}
};
void setup() 
{
  Serial.begin(2000000);
  time.begin();
  timing = EEPROM.read(0);
  pinMode(bBtn,INPUT_PULLUP);
  pinMode(sBtn,INPUT_PULLUP);
  pinMode(rLED, OUTPUT);
  pinMode(gLED, OUTPUT);
  lcd.init();
  
  lcd.setCursor(0,1);
  unsigned long _ = millis();
  while(millis() - _ <= 3000)
  {
    if((uint8_t)Serial.read() == 228)
    {
      mode = 1;
      digitalWrite(gLED, HIGH);
      digitalWrite(rLED, HIGH);
      lcd.backlight();
      break;
    }
  }
}
void recv(byte num)
{
  for(int i = 0; i < num; i++)
  {
    
    uint8_t received_data = Serial.read();
    static byte msg = 0;
    static byte data[packet_size + 1];
    if(millis() - lastS >= 80)
      msg = 0;
    data[msg] = received_data;
    //lcd.print((String)((data[1] << 8) | data[2]) + (String)(data[0]));
    
    
    if(msg == packet_size)
    {
      time.settime(data[0], data[1], data[2], data[3], data[4], data[5], data[6]);
      EEPROM.update(0, data[7]);
      timing = data[7];
      msg = 0;
    }
    else
      msg++;
  }
  lastS = millis();
}
void loop() 
{
  
  if(mode == 1)
  {
    
    if(Serial.available())
    {
      recv(Serial.available());
    }
    if(millis() % 200 == 0)
      callback();
  }
  else if(millis() - lCheck >= 500)
  {
    if(bLastState != digitalRead(bBtn))
    {
      if(bLastState == 1)
      {
        bLight = !bLight;
        if(bLight)
          lcd.backlight();
        else
          lcd.noBacklight();
      }
      bLastState = digitalRead(bBtn);
    }
    lcd.setCursor(0, 1);
    lcd.print(TEXT.substring(text_pos, text_pos + 7));
    text_pos += 1;
    if(text_pos == TEXT.length())
      text_pos = 0;
    if(digitalRead(sBtn))
    {
      digitalWrite(gLED, HIGH);
      digitalWrite(rLED, LOW);
    }
    else
    {
      digitalWrite(gLED, LOW);
      digitalWrite(rLED, HIGH);
    }
    if(min_ac != time.minutes)
    {
      hour_ac = time.Hours;
      min_ac = time.minutes;
      int w_day = time.weekday;
      if(w_day != 6 && w_day != 7 && digitalRead(sBtn))
      {
        for(int i = 0; i < tt_size; i++)
        {
          if(main_set[i][0] == hour_ac && main_set[i][1] == min_ac)
          {
            r_state = true;
            stopTimer = millis();
            beepFlag = true;
            break;
          }
        }
      }
      else if(w_day == 6 && digitalRead(sBtn))
      {
        for(int i = 0; i < tt_size; i++)
        {
          
          if(sat_set[i][0] == hour_ac && sat_set[i][1] == min_ac)
          {
            r_state = true;
            stopTimer = millis();
            beepFlag = true;
            break;
          }
        }
      }
    }
    if(r_state && ((millis() - stopTimer) >= timing * 100))
    {
      r_state = false;
    }
    lCheck = millis();
  }
  if(millis() % 100==0)
  {
    /*
    if(millis() % 2000==0)
      lcd.clear();
    */
    if(mode)
    {
      lcd.setCursor(0, 1);
      lcd.print("DBG");
    }
    lcd.setCursor(8, 1);
    lcd.print("T:" + String(timing / 10.0) + "s ");
    lcd.setCursor(0, 0);
    lcd.print(time.gettime("D-M H:i:s"));
  }
  if(digitalRead(rBtn))
    pinMode(rPin, HIGH);
  else
    pinMode(rPin, r_state);

}
void callback()
{
  Serial.write(time.seconds);
  Serial.write(time.minutes);
  Serial.write(time.Hours);
  Serial.write(time.day);
  Serial.write(time.month);
  Serial.write(time.year);
  Serial.write(time.weekday);
  Serial.write(timing);
}
