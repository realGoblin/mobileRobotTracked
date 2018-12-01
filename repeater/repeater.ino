#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Wire.h"
const uint64_t pipe2 = 0xF0F1F2F3F1LL;
RF24 radio(9, 10); // CE, CSN
int data = 0;
String flags = " ";
int flag  = 0;
String resutX;
String resutY;
int xAngle  = 0;
int yAngle  = 0;
void setup() {
  //открытие Serial-порта со скоростью 9600 бод/c
  Serial.begin(9600);

  //настройка пина со светодиодом в режим выхода
  pinMode(13, OUTPUT);
  radio.begin();
  delay(2);
  radio.setChannel(9); 
  radio.setDataRate(RF24_1MBPS); 
   
    radio.setPALevel(RF24_PA_HIGH);   
  radio.openWritingPipe(pipe2); 
}

void loop() {

  if (Serial.available() != 0) {  
    // ss=L128/T13/1235;
    //парсим строку
    //int in = ss.indexOf("x"); //индекс начала (маркера)
    //resut = ss.substring(in+1,in+3);//начинаем после паркира и берём 2 символа
    // Serial.println(resut);
    
     String ss = Serial.readString();     
     int in = ss.indexOf("x"); //индекс начала (маркера)
     resutX = ss.substring(in+1,in+4);//начинаем после паркира и берём 2 символа
     in = ss.indexOf("y"); //индекс начала (маркера)
     resutY = ss.substring(in+1,in+4);//начинаем после паркира и берём 2 символа
     xAngle=atoi(resutX.c_str());//из Sting в it
     yAngle=atoi(resutY.c_str());//из Sting в it
     
      if ( xAngle > 70)data = 101;
      else if ( xAngle < 30)data = 111;
      else if ( yAngle > 70 )data = 121;
      else if ( yAngle < 30 )data = 131;
      else data =  0;
      Serial.println(data);
      // Serial.println(yAngle);
      // Serial.println(data);
    radio.write(&data, sizeof(data));
    
}
}





