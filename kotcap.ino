#include "BluetoothSerial.h"
#include <WiFi.h>
int servopin=13; 
int myangle; 
int pulsewidth; 
int target;
int pos;
int dis;
String cmdstring;
const char *ssid = "KOTCAP_WIFI";
const char *password = "";
String header;
String capstate = "close";
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
        //clearSerialPort(); 
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
        //clearBTSerialPort(); 
    }
}

void webEvent()
{
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    cmdstring = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (cmdstring.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // set target to 0 or 180
            if (header.indexOf("GET /open") >= 0) {
              Serial.println("Lens cap is open");
              capstate = "open";
              target =180;
            } else if (header.indexOf("GET /close") >= 0) {
              Serial.println("Lens cap is close");
              capstate = "close";
              target =0;
            }
            

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 50px 100px;");
            client.println("text-decoration: none; font-size: 50px; margin: 10px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            

            client.println("<body><h1>KOTCAP</h1>");
            
            // Display current state
            client.println("<p>Lens cap is " + capstate + ". Press button to </p>");  
            if (capstate=="close") {
              client.println("<p><a href=\"/open\"><button class=\"button\">Open</button></a></p>");
            } else {
              client.println("<p><a href=\"/close\"><button class=\"button button2\">Close</button></a></p>");
            } 
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            cmdstring = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          cmdstring += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
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
