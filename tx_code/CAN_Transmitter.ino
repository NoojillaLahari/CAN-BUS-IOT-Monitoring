/*
  ESP32 CAN Tx
*/

#include <LiquidCrystal.h>
#include <stdio.h>
//LiquidCrystal lcd(6, 7, 5, 4, 3, 2);
LiquidCrystal lcd(13, 12, 14, 27, 26, 25);

#include <SPI.h>
#include <mcp2515.h>


struct can_frame canMsg;
MCP2515 mcp2515(5); // CS pin

#define CAN_TX_ID 0x036

#include "DHT.h"
#define DHTPIN 21    
#include "DHT.h"
#define DHTTYPE DHT11
//#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

int lvl1 = 22;
int lvl2 = 15;

int fire   = 16;
int ir     = 17;
int buzzer = 2;

int tempc=0,humc=0;

void setup() 
{
  Serial.begin(115200);

  pinMode(lvl1, INPUT_PULLUP);  pinMode(lvl2, INPUT_PULLUP);
  pinMode(fire, INPUT); pinMode(ir, INPUT);
  pinMode(buzzer, OUTPUT);

  digitalWrite(buzzer, HIGH);
  
  lcd.begin(16, 2);  
  lcd.print("IOT Centralised");
  lcd.setCursor(0,1);
  lcd.print("Realtime Passenger");
  delay(2500);
  
  lcd.clear();
  lcd.print("Bus Monitoring");
  lcd.setCursor(0,1);
  lcd.print("  System ");
  delay(2500);
  
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
  dht.begin();

  lcd.clear();        lcd.print("T:");//2-3-4,0
  lcd.setCursor(5,0); lcd.print("H:");//7-8-9,0
  lcd.setCursor(10,0);lcd.print("L:");//12-13-14,0

  lcd.setCursor(0,1); lcd.print("F:");//2-3-4,1  
  lcd.setCursor(8,1); lcd.print("I:");//10-11-12,1
  
  Serial.println("CAN TX Ready");
}



void loop() 
{ 
  canMsg.can_id  = 0x036;           //CAN id as 0x036
  canMsg.can_dlc = 8;               //CAN data length as 8
        
   tempc = dht.readTemperature();
   humc  = dht.readHumidity();
   
   lcd.setCursor(2,0);convertl(tempc);
   lcd.setCursor(7,0);convertl(humc);
   
   canMsg.data[0] = tempc;               //Update humidity value in [0]
   canMsg.data[1] = humc;               //Update temperature value in [1]

    if(tempc >= 40 || humc >= 90)
      {
        digitalWrite(buzzer, LOW); 
      }
      
    if(digitalRead(lvl1) == HIGH && digitalRead(lvl2) == HIGH) 
      {
        lcd.setCursor(12,0);lcd.print("Emp");
        canMsg.data[2] = 0;
      }
    if(digitalRead(lvl1) == LOW && digitalRead(lvl2) == HIGH) 
      {
        lcd.setCursor(12,0);lcd.print("-1 ");
        canMsg.data[2] = 1;
      }
    if(digitalRead(lvl1) == LOW && digitalRead(lvl2) == LOW) 
      {
        lcd.setCursor(12,0);lcd.print("Ful");
        canMsg.data[2] = 2;
      }

    if(digitalRead(fire) == LOW)
      {
        lcd.setCursor(2,1);lcd.print("ON ");    
        canMsg.data[3] = 3;
        digitalWrite(buzzer, LOW);
      }
    if(digitalRead(fire) == HIGH)
      {
        lcd.setCursor(2,1);lcd.print("OFF");    
        canMsg.data[3] = 4;
      }
    delay(100);
    
    if(digitalRead(ir) == LOW)
      {
        lcd.setCursor(10,1);lcd.print("ON ");    
        canMsg.data[4] = 5;
      }
    if(digitalRead(ir) == HIGH)
      {
        lcd.setCursor(10,1);lcd.print("OFF");    
        canMsg.data[4] = 6;
      }
    delay(100);
    if(tempc <= 39 && humc <= 89 && digitalRead(fire) == HIGH)
      {
         digitalWrite(buzzer, HIGH);    
      }
      
  canMsg.data[5] = 0x00;
  canMsg.data[6] = 0x00;
  canMsg.data[7] = 0x00;

  mcp2515.sendMessage(&canMsg);     //Sends the CAN message
  

  //Serial.print("Sent Temp: ");
  //Serial.println(tempC);

  delay(1000);
}



void converts(unsigned int value)
{
  unsigned int a,b,c,d,e,f,g,h;

      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;


      a=a|0x30;               
      c=c|0x30;
      e=e|0x30; 
      g=g|0x30;              
      h=h|0x30;
    
     
   Serial.write(a);
   Serial.write(c);
   Serial.write(e); 
   Serial.write(g);
   Serial.write(h);
}

void convertl(unsigned int value)
{
  unsigned int a,b,c,d,e,f,g,h;

      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;


      a=a|0x30;               
      c=c|0x30;
      e=e|0x30; 
      g=g|0x30;              
      h=h|0x30;
    
     
   //lcd.write(a);
   //lcd.write(c);
   lcd.write(e); 
   lcd.write(g);
   lcd.write(h);
}
