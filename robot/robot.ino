#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "I2Cdev.h"
#include <SoftwareSerial.h>
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif
//#define OUTPUT_READABLE_ACCELGYRO

const uint64_t pipe2 = 0xF0F1F2F3F1LL;// индитификатор передачи, "труба"

RF24 radio(9, 10); // CE, CSN
int data = 0;

#define FORW 101
#define BACK 111
#define STOP 0
#define LEFTT 121
#define RIGHTT 131
#define OFFSTOP 141
int IN3 = 5; // Input3 
int IN4 = 4; //задний двигатель
int ENB = 3;
int IN1 = 8; // Input1
int IN2 = 7; //передний двигатель
int ENA = 6;
int flag = 141;
boolean stopflag = 0;
int OutputSignal;
unsigned long currentMillis; 
long previousMillis1 = 0; 
//long previousMillis2 = 0; 
int time1 = 0; 
//String ss = "L128/T13/1235";
 String resut = "";

void setup() {
  
 
  Serial.begin(9600);
  // пины драйвера  
  pinMode (ENB, OUTPUT); 
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (ENA, OUTPUT); 
  //начало радиосвязи
   radio.begin();
  delay(2);
  //выбор канала радиосвязи
  radio.setChannel(9); // (0-127)
     // скорость, RF24_250KBPS, RF24_1MBPS или RF24_2MBPS
      // RF24_250KBPS на nRF24L01 (без +) неработает.
      // меньше скорость, выше чувствительность приемника.
  radio.setDataRate(RF24_1MBPS); 
   
   // мощьность передатчика RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM,
  radio.setPALevel(RF24_PA_HIGH);   
// открывает режим передачи
 //открывает режим прослушки 
   radio.openReadingPipe(1,pipe2);
   //начинаем с прослушки
  radio.startListening();
}

 
// подпрограмма перезапуска ардуино
void(* resetFunc) (void) = 0;

void loop() {
    RADIO();
TIMEBREAK();
//получение времени
     currentMillis = millis();

   if (flag == FORW) {
      RotationForw();   
    } 
       if  (flag == BACK) {
      RotationBack();  
    } 
 
      if  (flag == LEFTT) {         
      RotationLeft();
    } 
       if  (flag == RIGHTT) {
      RotationRight();     
    } 
         if  (flag == OFFSTOP) {
      RotationStop();
    }
 
  }
 void RotationForw() {
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(ENA,105);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);  
  analogWrite(ENB,105);
 }
 
  void RotationBack() {
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(ENA,105);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);  
  analogWrite(ENB,105);
 }
 void RotationLeft() {
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);  
  analogWrite(ENA,105);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);  
  analogWrite(ENB,105);
 }
  void RotationRight() {
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(ENA,105);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);  
  analogWrite(ENB,105);
 }
 void RotationStop() {
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
 }
 
void RADIO() {

   if (radio.available()){ // проверяем не пришло ли чего в буфер.
    radio.read(&data, sizeof(data)); 
    // читаем данные, указываем сколько байт читать
    flag = data;
     previousMillis1 = currentMillis;
   
    Serial.println(flag);

       // ss=L128/T13/1235;
       //парсим строку
      // int in = ss.indexOf("T"); //индекс начала (маркера)
      // resut = ss.substring(in+1,in+3);//начинаем после паркира и берём 2 символа
      // Serial.println(resut);
  }
}
  void TIMEBREAK() {
     time1 = currentMillis - previousMillis1;
     Serial.println(time1);
    if ( time1 > 1000) {
      resetFunc();
  }
  }


