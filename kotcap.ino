int servopin=32; 
int myangle; 
int pulsewidth; 
int target;
int pos;
int nextpos;
int dis;
String comstring;

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

void serialEvent()
{
while (Serial.available())  
    {
        comstring += char(Serial.read());
        delay(5);
    }

if (comstring.length() > 0 )
    {
      Serial.print("received cmd: ");Serial.println(comstring);
        target = comstring.toInt();
      Serial.print("covert to int target: ");Serial.println(target);
        comstring = "";
        clearSerialPort(); 
    }
}

void setup()
{
pinMode(servopin,OUTPUT);
Serial.begin(115200);
target = 0;
pos = 0;
}

void loop()
{
serialEvent();

if (target>pos && target<=180)
{    
 dis = target-pos;
  for(int i=0;i<dis;i++)
  {
    nextpos = pos +1;
    servopulse(servopin,nextpos);
    delay(20);
    pos =nextpos;
  }
} 

 if (target<pos && target>=0)
{  
 dis = pos -target;
  for(int i=0;i<dis;i=i+1)
  {servopulse(servopin,pos-1);
    delay(20);
    pos =pos -1;
  }
} 
} 
