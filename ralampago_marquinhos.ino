#include <Servo.h>

//    --- servos ---

#define servo1 2     //Left Servo
#define servo2 12     //Right Servo 

//    --- sensors ---

#define Lsens 5
#define Msens 7
#define Rsens 9

Servo Lservo;
Servo Rservo;

int RSS;    // Right Servo Speed
int LSS;    // Left Motor Speed

int Left_IR;
int Middle_IR;
int Right_IR;

int error;
long agora;
long antes;
long dif2;
int diferenca;

int PError;
float C;
float Ki = .1;
long ErrTotal = 0;

String Sensor_Value = ""; // get the 0's and 1's

void setup()
{
  Serial.begin(9600);
  Lservo.attach(servo1);
  Rservo.attach(servo2);
}

void loop()
{
  Left_IR = analogRead(Lsens);
  Middle_IR = analogRead(Msens);
  Right_IR = analogRead(Rsens);

  Time();
  Sensor();
  SetSpeed();
  Correcao();
  Drive();
  
  Display();
  delay(1);
}

//    --- functions---
    
void Sensor()
{
  Sensor_Value = "";
  Sensor_Value += digitalRead(Lsens);
  Sensor_Value += digitalRead(Msens);
  Sensor_Value += digitalRead(Rsens);
}


void Time()
{
   agora = millis();
   diferenca = agora - antes;
   antes = agora;
}

void SetSpeed()
{
  if(Sensor_Value =="000")
  {
   LSS = 1580;
   RSS = 1400;
   if(error > 0)
     {
        error = 3;
     }
   else if(error < 0)
     {
        error = -3;
     } else if (error == 0)
     {
        error = 0;
     }
  }
  
  else if(Sensor_Value =="001")
  {
   LSS = 1580;
   RSS = 1520;
   error = 2;
  }
  
  else if(Sensor_Value =="010")
  {
   LSS = 1580;
   RSS = 1410;  
   error = 0;  
  }
  
  else if(Sensor_Value =="100")
  {
   LSS = 1480;
   RSS = 1400; 
   error = -2;  
  }
  
  else if(Sensor_Value =="011")
  {
   LSS = 1680;
   RSS = 1530;
   error = 1;    
  }
  
  else if(Sensor_Value =="101")
  {
   LSS = 1580;
   RSS = 1410;
   error = 0;   
  }
  
  else if(Sensor_Value =="111")
  {
   LSS = 1580;
   RSS = 1410;
   error = 0;    
  }
  
  else if(Sensor_Value =="110")
  {
   LSS = 1530; 
   RSS = 1310;
   error = -1;    
  }
}

void Correcao()
{
   if(PError*error < 0 or PError*error==0)
   {
     ErrTotal = 0;
   }
   
   ErrTotal += diferenca*error;
   C = Ki*ErrTotal;
   PError = error;
}

void Drive()
{
   Lservo.writeMicroseconds(LSS + C);
   Rservo.writeMicroseconds(RSS + C);
}

void Display() 
{
  if (agora-dif2 > 150)
  {
    Serial.print("Sensor_Value: ");  Serial.println(Sensor_Value);
    Serial.print("error: ");Serial.println(error);
    Serial.print("Ki: ");Serial.println(Ki);
    Serial.print("ErrTotal: ");Serial.println(ErrTotal);
    Serial.print("C: ");Serial.println(C);
    Serial.println();
    dif2 = agora;
  }
}
