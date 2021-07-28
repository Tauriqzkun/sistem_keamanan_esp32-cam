#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Wifi network station credentials
//#define WIFI_SSID "kun"
//#define WIFI_PASSWORD "rican0987"

// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "1713777755:AAHGazdfcLMsfUfD-8Kf2xs5TkmzkQgazVE"
String CHAT_ID = "600848146";
const unsigned long BOT_MTBS = 1000; // mean time between scan messages

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

int waktu1=300;
int waktu0=200;

const int led_dan_buzzer = D5;
const int ledPin2 = 16;
const int bu = 0;

const int sensor1 = D6;
const int sensor2 = D7;

//const int servo =  D7;
void setup()
{
  Serial.begin(115200);
    WiFi.mode(WIFI_STA);
  Serial.println();
pinMode(sensor1, INPUT_PULLUP);
pinMode(sensor2, INPUT_PULLUP);

pinMode(led_dan_buzzer, OUTPUT); // initialize digital ledPin as an output.
pinMode(ledPin2, OUTPUT);
pinMode(bu, OUTPUT);
 secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  Serial.print("Connecting to Wifi SSID ");
  
   WiFiManager wm;
   bool res;
   res = wm.autoConnect("SISTEM KEAMANAN 2");
  if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }
  
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
 String AWAL = "SELAMAT DATANG DI SYSTEM\n";
      AWAL += "KEAMANAN RUMAH BERBASIS IoT \n";
      AWAL += "Silangkan klik /start \n";
      AWAL += "TERIMAKASIH  \n";
      bot.sendMessage(CHAT_ID, AWAL, "");
}

void loop()
{
  
int kondisiSensor1 = digitalRead(sensor1);
int kondisiSensor2 = digitalRead(sensor2);

if(kondisiSensor1 == LOW)
    {
 digitalWrite(led_dan_buzzer,HIGH);//6
 delay(1000);
 digitalWrite(led_dan_buzzer,LOW);
 delay(100);
 Serial.println("led1");   
 bot.sendMessage(CHAT_ID, "Sensor pintu terdeteksi  ", "");
 Serial.println("on");
digitalWrite(led_dan_buzzer,HIGH);//1
      delay(waktu1);
       digitalWrite(led_dan_buzzer,LOW);
      delay(waktu0);
             digitalWrite(led_dan_buzzer,HIGH);//2
      delay(waktu1);
      digitalWrite(led_dan_buzzer,LOW);
    delay(waktu0);
             digitalWrite(led_dan_buzzer,HIGH);//3
      delay(waktu1);
      digitalWrite(led_dan_buzzer,LOW);
      delay(waktu0);
            digitalWrite(led_dan_buzzer,HIGH);//4
      delay(waktu1);
       digitalWrite(led_dan_buzzer,LOW);
       delay(waktu0);
            digitalWrite(led_dan_buzzer,HIGH);//5
      delay(waktu1);
       digitalWrite(led_dan_buzzer,LOW);
       delay(waktu0);
              digitalWrite(led_dan_buzzer,HIGH);//6
      delay(waktu1);
       digitalWrite(led_dan_buzzer,LOW);
       delay(waktu0);
              digitalWrite(led_dan_buzzer,HIGH);//7
      delay(waktu1);
       digitalWrite(led_dan_buzzer,LOW);
       delay(waktu0);
              digitalWrite(led_dan_buzzer,HIGH);//8
       delay(waktu1);
       digitalWrite(led_dan_buzzer,LOW);
        delay(waktu0);
              digitalWrite(led_dan_buzzer,HIGH);//9
      delay(waktu1);
       digitalWrite(led_dan_buzzer,LOW);
        delay(waktu0);
              digitalWrite(led_dan_buzzer,HIGH);//10
       delay(waktu1);
       digitalWrite(led_dan_buzzer,LOW);
       delay(waktu0);
}

if(kondisiSensor2 == LOW)
    {
 digitalWrite(ledPin2,HIGH);//6
 delay(300);
 digitalWrite(ledPin2,LOW);
 delay(100);
 
bot.sendMessage(CHAT_ID, "Sensor dalam mati ", "");
Serial.println("Sensor dalam mati ");
digitalWrite(bu,HIGH);
delay(waktu1);
digitalWrite(bu,LOW);
delay(waktu0);
digitalWrite(bu,HIGH);
delay(waktu1);
digitalWrite(bu,LOW);
delay(waktu0);
digitalWrite(bu,HIGH);
delay(waktu1);
digitalWrite(bu,LOW);
delay(waktu0);
digitalWrite(bu,HIGH);
delay(waktu1);
digitalWrite(bu,LOW);
delay(waktu0);
digitalWrite(bu,HIGH);
delay(waktu1);
digitalWrite(bu,LOW);
delay(waktu0);
digitalWrite(bu,HIGH);
delay(waktu1);
digitalWrite(bu,LOW);
delay(waktu0);
digitalWrite(bu,HIGH);
delay(waktu1);
digitalWrite(bu,LOW);
delay(waktu0);
digitalWrite(bu,HIGH);
delay(waktu1);
digitalWrite(bu,LOW);
delay(waktu0);
digitalWrite(bu,HIGH);
delay(waktu1);
digitalWrite(bu,LOW);
delay(waktu0);
digitalWrite(bu,HIGH);
delay(waktu1);
digitalWrite(bu,LOW);
delay(waktu0);
digitalWrite(bu,HIGH);
delay(waktu1);
digitalWrite(bu,LOW);
delay(waktu0);
 Serial.println("led2");   
// bot.sendMessage(CHAT_ID, "Sensor dalam mati ", "");
 Serial.println("Sensor dalam mati ");
//delay (5000);
}
else
{ // goes from 180 degrees to 0 degrees
  Serial.println("off");
  digitalWrite(ledPin2,HIGH);
}

}
