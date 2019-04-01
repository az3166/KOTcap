#include "BluetoothSerial.h"
#include <WiFi.h>
int servopin=33; 
int myangle; 
int pulsewidth; 
int target;
int pos;
int dis;
String cmdstring;
const char *ssid = "KOTCAP_WIFI";
const char *password = "";
String header;
String capstate = "Close";
WiFiServer server(80);
BluetoothSerial BTSerial;

void servopulse(int servopin,int myangle)
{
pulsewidth=(myangle*11)+500;
digitalWrite(servopin,HIGH);
delayMicroseconds(pulsewidth);
digitalWrite(servopin,LOW);
delay(20-pulsewidth/1000);
}

void clearSerialPort()
{
  while (Serial.available())
  {
    Serial.read();
  }
}

void clearBTSerialPort()
{
  while (BTSerial.available())
  {
    BTSerial.read();
  }
}

void serialEvent()
{
while (Serial.available())  
    {
        cmdstring += char(Serial.read());
        delay(5);
    }
if (cmdstring.length() > 0 )
    {
        target = cmdstring.toInt();
        cmdstring = "";
        clearSerialPort(); 
    }
}

void BTSerialEvent()
{
while (BTSerial.available())  
    {
        cmdstring += char(BTSerial.read());
        delay(5);
    }
if (cmdstring.length() > 0 )
    {
        target = cmdstring.toInt();
        cmdstring = "";
        clearBTSerialPort(); 
    }
}

void webEvent()
{
WiFiClient client = server.available();   
  if (client) {                             
    while (client.connected()) {   
      if (client.available()) {   
        char c = client.read();            
        header += c;
        if (c == '\n') {    
          if (cmdstring.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("GET /180") >= 0) {
              capstate = "Open 180&deg";
              target = 180;
            } else if (header.indexOf("GET /90") >= 0) {
              capstate = "Open 90&deg";
              target = 90;
            } else if (header.indexOf("GET /0") >= 0) {
              capstate = "Close";
              target = 0;
            } 
            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; width: 80%; border-radius: 12px;font-size: 24px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            client.println("<body><h1>KOTCAP</h1>");
            client.println("<p>Telescope lens cap is " + capstate +".</p>");
            client.println("<p><a href=\"/180\"> <button class= \"button\">Open 180&deg</button></a></p>");
            client.println("<p><a href=\"/90\"> <button class=\"button\">Open 90&deg</button></a></p>");
            client.println("<p><a href=\"/0\"> <button class=\"button button2\">Close</button></a></p>");
            client.println("</body></html>");

            client.println();
            break;
          } else { 
            cmdstring = "";
          }
        } else if (c != '\r') {  
          cmdstring += c;     
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void setup()
{
pinMode(servopin,OUTPUT);
Serial.begin(115200);
BTSerial.begin("KOTCAP_BT");
WiFi.mode(WIFI_AP); 
while(!WiFi.softAP(ssid, password)){}; 
WiFi.softAPsetHostname("KOTCAP"); 
server.begin();
target = 0;
pos = 0;
}

void loop()
{
serialEvent();
BTSerialEvent(); 
webEvent();

if (target>pos && target<=180)
{    
 dis = target -pos;
  for(int i=0;i<dis;i++)
  {
    servopulse(servopin,pos+1);
    delay(20);
    pos =pos +1;
  }
} 

 if (target<pos && target>=0)
{  
 dis = pos -target;
  for(int i=0;i<dis;i++)
  {
    servopulse(servopin,pos-1);
    delay(20);
    pos =pos -1;
  }
} 
} 