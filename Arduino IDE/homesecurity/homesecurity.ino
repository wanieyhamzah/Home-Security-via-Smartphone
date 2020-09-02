#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#define buzzer 12

char auth[] = "jQ50GKHlsyTuFcWYmSVGoILpgpfDDNK8";
char ssid[] = "wifi";
char pass[] = "password";

int flag = 0;
int flag1 = 0;

#define EspSerial Serial1
#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);
WidgetTable table;
BLYNK_ATTACH_WIDGET(table, V4);

int count = 0;
int x = 1;
int y = 2;

WidgetRTC rtc;
BlynkTimer timer;

BLYNK_CONNECTED(){
  rtc.begin();
}

BLYNK_WRITE(V0) {                       //Fan switch
 int buttonState = param.asInt();
  if (buttonState == 1){
    digitalWrite(10,LOW);
  }
  else {
    digitalWrite(10,HIGH);
  }
}

BLYNK_WRITE(V1) {                       //Door lock
 int buttonState = param.asInt();
  if (buttonState == 1){
    digitalWrite(9,LOW);
  }
  else {
    digitalWrite(9,HIGH);
  }
}

BLYNK_WRITE(V2) {                       //Lights switch
 int buttonState = param.asInt();
  if (buttonState == 1){
    digitalWrite(8,LOW);
  }
  else {
    digitalWrite(8,HIGH);
  }
}

BLYNK_WRITE(V3) {                       //Notification
 int buttonState = param.asInt();
  if (buttonState == 1){
    flag = 0;
  }
  else {

  }
}

BLYNK_WRITE(V5) {                        //Off sensor
 int buttonState = param.asInt();
  if (buttonState == 1){
    flag1 = 0;
  }
  else {
    flag1 = 1;
  }
}

void setup() {
  Serial.begin(9600);
  setSyncInterval(10*60);
  pinMode(11,INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(8,OUTPUT);
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  Blynk.begin(auth, wifi, ssid, pass);
  digitalWrite(10,HIGH);
  digitalWrite(9,HIGH);
  digitalWrite(8,HIGH);
}

void loop() {
  Blynk.run();
  timer.run();
  String currentDate = String(day()) + ":" + month() + ":" + year();
  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  delay(100);
   int irsensor = digitalRead(11);
  Serial.println(irsensor);
  
  if (irsensor == LOW && flag1 == 0){
    flag = 1;
    count++;
    Blynk.notify("WARNING!! Someone in your house! Detected:" + String(count));
    table.addRow(x, "SENSOR" , count);
    table.addRow(y, "DATE" , currentDate);
    delay(1000);
  } else if (hour()== 23 && minute() == 59 && second() == 59){
    x = x + 2;
    y = y + 2;
    delay(1000);
  } else if (flag == 1){
    digitalWrite(buzzer,HIGH);
  } else {
    digitalWrite(buzzer,LOW);
  }
}
