#include <iarduino_RTC.h>
#include <EEPROM.h>
#include <Firmata.h>
#define RelayPin 8
#define switcherPin 5
#define buttonPin 2

iarduino_RTC time(RTC_DS3231);

volatile bool Sync = false, beepFlag = false, oneBeepFlag = 0, switchFlag = 1;
volatile byte l_time[6], timing = 0;
unsigned long syncTime = 0, lastSend = 0, stopTimer = 0;
volatile int counter = 0, hour_ac = 0, min_ac = 0;

const byte set[14][2] =
{
  {8, 0},
  {9, 30},
  {9, 40},
  {11, 10},
  {11, 20},
  {12, 50},
  {13, 00},
  {14, 30},
  {14, 40},
  {16, 20},
  {17, 50},
  {18, 00},
  {19, 30}
};
/*
const byte set[14][2] =
{
  {16, 56},
  {16, 57},
  {16, 58},
  {15, 29},
  {15, 20},
  {22, 18},
  {13, 00},
  {14, 30},
  {14, 40},
  {16, 20},
  {17, 50},
  {18, 00},
  {19, 30}
};
*/
void setup() 
{
  pinMode(RelayPin, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(switcherPin, INPUT_PULLUP);
  digitalWrite(7, HIGH);
  time.begin();
  timing = EEPROM.read(0);
  Firmata.setFirmwareVersion(FIRMATA_MAJOR_VERSION, FIRMATA_MINOR_VERSION);
  Firmata.begin(115200);
  Firmata.attach(ANALOG_MESSAGE, inputSync);
  while (Firmata.available())
  {
    Firmata.processInput();
  }
  //Serial.begin(115200);
  //Serial.println(timing);
  timerInit();
}

void loop() 
{
  if(digitalRead(switcherPin))
  {
    switchFlag = 1;
    if (Firmata.available() > 0)
    {
      if(!Sync) timing = EEPROM.read(0);
      Sync = true;
      syncTime = millis();
    }
    if(beepFlag && millis() - stopTimer >= timing * 100)
    {
      digitalWrite(RelayPin , LOW);
      //Serial.println("Off");
      beepFlag = false;
    }
    Syncing();
  }
  else
  {
    digitalWrite(RelayPin, digitalRead(buttonPin));
  }
}
void Syncing()
{
  if (Sync)
  {
    while (Firmata.available()) 
    {
      Firmata.processInput();
    }
    FirmataSend();
    if (millis() - syncTime > 700) {
      Sync = false;
    }
  }
}
void FirmataSend()
{
  if(millis() - lastSend >= 100)
  {
    time.begin();
    Firmata.sendAnalog(1, time.Hours);
    Firmata.sendAnalog(2, time.minutes);
    Firmata.sendAnalog(3, time.seconds);
    Firmata.sendAnalog(4, timing);
    lastSend = millis();
  }
}
void inputSync(byte wave, int val)
{
  if(wave >= 0 && wave <= 5) 
  {
    l_time[wave] = val;
    time.settime(l_time[0], l_time[1], l_time[2], l_time[3], l_time[4], l_time[5]);
  }
  if(wave == 6 && timing != val){EEPROM.update(0, val); timing = EEPROM.read(0);}
}

void period()
{
  time.begin();
  if(hour_ac != time.Hours || min_ac != time.minutes)
  {
    hour_ac = time.Hours;
    min_ac = time.minutes;
    for(int i = 0; i < 14; i++)
    {
      if(set[i][0] == hour_ac && set[i][1] == min_ac)
      {
        digitalWrite(RelayPin , HIGH);
        oneBeepFlag = true;
        stopTimer = millis();
        beepFlag = true;
        break;
      }
    }
  }
  
}

void timerInit()
{
  TCCR0A |= (1 << WGM01);
  OCR0A = 0xF9;
  TIMSK0 |= (1 << OCIE0A);
  TCCR0B |= (1 << CS01) | (1 << CS00);
  sei();
}

ISR (TIMER0_COMPA_vect)
{
  counter++;
  if(counter >= 2000 && switchFlag)
  {
    counter = 0;
    period();
  }
}
