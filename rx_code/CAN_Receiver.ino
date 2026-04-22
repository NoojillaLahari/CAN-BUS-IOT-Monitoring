/*
  ESP32 CAN Rx
*/

#include <LiquidCrystal.h>
#include <stdio.h>
//LiquidCrystal lcd(6, 7, 5, 4, 3, 2);
LiquidCrystal lcd(13, 12, 14, 27, 26, 25);

#include <ESP32Servo.h>
Servo myservo;  // create servo object to control a servo

#include <WiFi.h>
#include <HTTPClient.h>

HTTPClient http;

const char *ssid = "iotserver";
const char *password = "iotserver123";

int httpResponseCode;
String servername = "http://projectsfactoryserver.in/storedata.php?name=";
String accountname = "iot1560";
String field1 = "&lat=";
String field2 = "&lan=";
String field3 = "&s1=";
String field4 = "&s2=";
String field5 = "&s3=";
String field6 = "&s4=";
String field7 = "&s5=";
String payload="";

int pump = 21;

#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;
MCP2515 mcp2515(5);

#define CAN_TX_ID  0x036
#define CAN_ACK_ID 0x037

int tempc=0,humc=0;
int lvlv=0;
int firev=0;
int irv=0;

String lvl_string="";
String fire_string="";
String ir_string="";



int i=0,k=0,lop=0;
int  gps_status=0;
float latitude=0; 
float logitude=0;                       
String Speed="";
String gpsString="";
char *test="$GPRMC";


//int hbtc=0,hbtc1=0,rtrl=0;

unsigned char gv=0,msg1[10],msg2[11];
 float lati=0,longi=0;
unsigned int lati1=0,longi1=0;
unsigned char flat[5],flong[5];
char finallat[10],finallong[10];
String finallat1="";
String finallong1="";

 int ii=0,rchkr=0;

void iot_send()
{
        lcd.setCursor(15,1);lcd.print("U");
 http.begin(servername + accountname + field1 + finallat1 + field2 + finallong1 + field3 + String(tempc) + field4 + String(humc) + field5 + lvl_string + field6 + fire_string + field7 + ir_string);
 //http.begin(servername + accountname + field1 + finallat1 + field2 + finallong1 + field3 + String(tempc) + field4 + String(hbv) + field5 + st_string);
      
        httpResponseCode = http.GET();
        delay(5000);        
        lcd.setCursor(15,1);lcd.print(" ");
}

void setup() 
{
  Serial.begin(9600);
  
  pinMode(pump, OUTPUT);
  digitalWrite(pump, LOW);
  
  myservo.attach(22);
  myservo.write(10);

  lcd.begin(16, 2);  
  lcd.print(" IOT Centralised");
  lcd.setCursor(0,1);
  lcd.print("Realtime Passenger");
  delay(2500);
  
  lcd.clear();
  lcd.print("Bus Monitoring");
  lcd.setCursor(0,1);
  lcd.print("  System ");
  delay(2500);
  
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) 
       {
           delay(500);
       }
  delay(3000);

  
  get_gps();
  gps_convert();

    lcd.clear();
    lcd.setCursor(0,0);
     for(ii=0;ii<=6;ii++) lcd.write(finallat[ii]);
     
    lcd.setCursor(0,1);
     for(ii=0;ii<=7;ii++) lcd.write(finallong[ii]);    
  
  finallat[7] = '\0';
  finallong[8] ='\0';

  finallat1="";
  finallong1="";

  finallat1 = String(finallat);
  finallong1 = String(finallong);
  
  delay(3000);

  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();

  lcd.clear();        lcd.print("T:");//2-3-4,0
  lcd.setCursor(5,0); lcd.print("H:");//7-8-9,0
  lcd.setCursor(10,0);lcd.print("L:");//12-13-14,0

  lcd.setCursor(0,1); lcd.print("F:");//2-3-4,1  
  lcd.setCursor(8,1); lcd.print("I:");//10-11-12,1
}

int cntlmk=0;
void loop() 
{
  if(mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK)// && canMsg.can_id == CAN_TX_ID) 
    {
      tempc  = canMsg.data[0];
      humc   = canMsg.data[1];
      lvlv   = canMsg.data[2];
      firev  = canMsg.data[3];
      irv    = canMsg.data[4];
        
      lcd.setCursor(2, 0);convertl(tempc);
      lcd.setCursor(7, 0);convertl(humc);

      lvl_string=""; 
      if(lvlv == 0){lcd.setCursor(12, 0);lcd.print("Emp");lvl_string="Empty";}
      if(lvlv == 1){lcd.setCursor(12, 0);lcd.print("-1 ");lvl_string="1";}
      if(lvlv == 2){lcd.setCursor(12, 0);lcd.print("Ful");lvl_string="Full";}

      fire_string="";
      if(firev == 3){lcd.setCursor(2,1);lcd.print("ON ");fire_string="ON"; digitalWrite(pump, HIGH);}
      if(firev == 4){lcd.setCursor(2,1);lcd.print("OFF");fire_string="OFF";digitalWrite(pump, LOW);}

      ir_string="";
      if(irv == 5){lcd.setCursor(10,1);lcd.print("ON ");ir_string="ON";}
      if(irv == 6){lcd.setCursor(10,1);lcd.print("OFF");ir_string="OFF";}

      if(irv == 5){myservo.write(110);delay(2000);}
      if(irv == 6){myservo.write(10);}
       
      cntlmk++;
      if(cntlmk >= 20)
        {cntlmk=0;
          iot_send();
        }
    }
}



void gpsEvent()
{
  gpsString="";
  while(1)
  {
   //while (gps.available()>0)            //Serial incoming data from GPS
    while (Serial.available() > 0)
   {
    //char inChar = (char)gps.read();
    char inChar = (char)Serial.read();
     gpsString+= inChar;                    //store incoming data from GPS to temparary string str[]
     i++;
    // Serial.print(inChar);
     if (i < 7)                      
     {
      if(gpsString[i-1] != test[i-1])         //check for right string
      {
        i=0;
        gpsString="";
      }
     }
    if(inChar=='\r')
    {
     if(i>60)
     {
       gps_status=1;
       break;
     }
     else
     {
       i=0;
     }
    }
  }
   if(gps_status)
    break;
  }
}

void get_gps()
{
  lcd.clear();
  lcd.print("Getting GPS Data");
  lcd.setCursor(0,1);
  lcd.print("Please Wait.....");
 
   gps_status=0;
   int x=0;
   while(gps_status==0)
   {
    gpsEvent();
    int str_lenth=i;
    coordinate2dec();
    i=0;x=0;
    str_lenth=0;
   }
}

void coordinate2dec()
{
  String lat_degree="";
    for(i=17;i<=18;i++)         
      lat_degree+=gpsString[i];
      
  String lat_minut="";
     for(i=18;i<=19;i++)         
      lat_minut+=gpsString[i];
     for(i=21;i<=22;i++)         
      lat_minut+=gpsString[i];
      
  String log_degree="";
    for(i=29;i<=31;i++)
      log_degree+=gpsString[i];
  String log_minut="";
    for(i=32;i<=33;i++)
      log_minut+=gpsString[i];
    for(i=35;i<=36;i++)
      log_minut+=gpsString[i];
      
    Speed="";
    for(i=42;i<45;i++)          //extract longitude from string
      Speed+=gpsString[i];
      
     float minut= lat_minut.toFloat();
     minut=minut/60;
     float degree=lat_degree.toFloat();
     latitude=degree+minut;
     
     minut= log_minut.toFloat();
     minut=minut/60;
     degree=log_degree.toFloat();
     logitude=degree+minut;
}

void gps_convert()
{
  if(gps_status)
  {
 // Serial.println(gpsString);

  if(gpsString[0] == '$' && gpsString[1] == 'G' && gpsString[2] == 'P' && gpsString[3] == 'R' && gpsString[4] == 'M' && gpsString[5] == 'C')
    {
     // Serial.println("Don11111111111111111111111111111111111111111111111111111\r\n");      
    //  Serial.write(gpsString[18]);Serial.write(gpsString[19]);Serial.write(gpsString[20]);Serial.write(gpsString[21]);Serial.write(gpsString[22]);
     //lcd.setCursor(0,0);
     for(ii=0;ii<9;ii++)
       {
        //lcd.write(gpsString[19+ii]);    
        msg1[ii] = gpsString[19+ii];
        //Serial.write(msg1[ii]);
       }
       //Serial.println("\r\n");
     //lcd.setCursor(0,1);
      for(ii=0;ii<10;ii++)
       {
        //lcd.write(gpsString[32+ii]);
        msg2[ii] = gpsString[32+ii];    
       // Serial.write(msg2[ii]);
       }


              convlat(lati); convlong(longi);
        finallat[0] = msg1[0];
        finallat[1] = msg1[1];
        finallat[2] = '.';
        finallat[3] = flat[0]; finallat[4] = flat[1];finallat[5] = flat[2];finallat[6] = flat[3];finallat[7] = '\0';


        finallong[0] = msg2[0];
        finallong[1] = msg2[1];
        finallong[2] = msg2[2];
        finallong[3] = '.';
        finallong[4] = flong[0];finallong[5] = flong[1];finallong[6] = flong[2];finallong[7] = flong[3];finallong[8] = '\0'; 

   
    }
  }  
}

 void convlat(unsigned int value)  
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
   
  // dlcd(a);
//   dlcd(c);dlcd(e); dlcd(g);dlcd(h);//lcddata('A');//lcddata(' ');lcddata(' ');
    
         
               flat[0] = c;
               flat[1] = e;
               flat[2] = g;
             flat[3] = h;


          }

 void convlong(unsigned int value)  
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
   
  // dlcd(a);
//   dlcd(c);dlcd(e); dlcd(g);dlcd(h);//lcddata('A');//lcddata(' ');lcddata(' ');
    
         
               flong[0] = c;
               flong[1] = e;
               flong[2] = g;
               flong[3] = h;
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
