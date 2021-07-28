#include <Arduino.h>
#include "src/OV2640.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
OV2640 cam;
const char* ssid = "kun";
const char* password = "rican0987";

// Initialize Telegram BOT
String BOTtoken = "1713777755:AAHGazdfcLMsfUfD-8Kf2xs5TkmzkQgazVE";  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
String CHAT_ID = "600848146";

bool sendPhoto = false;

WiFiClientSecure clientTCP;
UniversalTelegramBot bot(BOTtoken, clientTCP);

#define FLASH_LED_PIN 4
#define sensor1 2
//#define led 13
#define led_dan_buzzer 14
bool flashState = LOW;
bool flag = 0;
int waktu1=300;
int waktu0=200;

//Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

String sendPhotoTelegram();
//CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

WebServer server(80);

const char HEADER[] = "HTTP/1.1 200 OK\r\n" \
                      "Access-Control-Allow-Origin: *\r\n" \
                      "Content-Type: multipart/x-mixed-replace; boundary=123456789000000000000987654321\r\n";
const char BOUNDARY[] = "\r\n--123456789000000000000987654321\r\n";
const char CTNTTYPE[] = "Content-Type: image/jpeg\r\nContent-Length: ";
const int hdrLen = strlen(HEADER);
const int bdrLen = strlen(BOUNDARY);
const int cntLen = strlen(CTNTTYPE);

void handle_jpg_stream(void)
{
  char buf[32];
  int s;

  WiFiClient client = server.client();

  client.write(HEADER, hdrLen);
  client.write(BOUNDARY, bdrLen);

  while (true)
  {
    if (!client.connected()) break;
    cam.run();
    s = cam.getSize();
    client.write(CTNTTYPE, cntLen);
    sprintf( buf, "%d\r\n\r\n", s );
    client.write(buf, strlen(buf));
    client.write((char *)cam.getfb(), s);
    client.write(BOUNDARY, bdrLen);
  }
}

const char JHEADER[] = "HTTP/1.1 200 OK\r\n" \
                       "Content-disposition: inline; filename=capture.jpg\r\n" \
                       "Content-type: image/jpeg\r\n\r\n";
const int jhdLen = strlen(JHEADER);

void handle_jpg(void)
{
  WiFiClient client = server.client();

  cam.run();
  if (!client.connected()) return;

  client.write(JHEADER, jhdLen);
  client.write((char *)cam.getfb(), cam.getSize());
}

void handleNotFound()
{
  String message = "Server is running!\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  server.send(200, "text / plain", message);
}
void configInitCamera(){
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  //init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;  //0-63 lower number means higher quality
    config.fb_count = 2;
  } 
  else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;  //0-63 lower number means higher quality
    config.fb_count = 1;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
  }

  // Drop down frame size for higher initial frame rate
  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_CIF);  // UXGA|SXGA|XGA|SVGA|VGA|CIF|QVGA|HQVGA|QQVGA
}

void handleNewMessages(int numNewMessages) {
  Serial.print("Handle New Messages: ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);

    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);
    
    String from_name = bot.messages[i].from_name;
    if (text == "/start") {
      String welcome = "Welcome , " + from_name + "\n";
     // welcome += "Use the following commands to interact with the ESP32-CAM \n";
      welcome += "/photo : New photo\n";
      welcome += "/flash : Flash  \n";
      welcome += "/sensor_ON : Sensor Aktif \n";
      welcome += "/sensor_OFF: Sensor Mati \n";
      welcome += "/AlamatIP : Alamat IP Stream atau LIVE \n";
      welcome += "/status : Cek Keadaan Sensor\n";
      bot.sendMessage(chat_id, welcome, "");
    }
    if (text == "/flash") {
      flashState = !flashState;
      digitalWrite(FLASH_LED_PIN, flashState);
      Serial.println("Change flash LED state");
    }
    if (text == "/photo") {      sendPhoto = true;
      Serial.println("New photo request");
      bot.sendMessage(chat_id, "New photo request", "");
    }
    if (text == "/sensor_ON"){
      flag = 1;
      bot.sendMessage(chat_id, "Sensor sudah aktif, Saat terjadi gerakan anda akan dikirimkan foto", "");
    }
  if (text == "/sensor_OFF"){
      flag = 0;
      bot.sendMessage(chat_id, "Sensor sudah mati, Anda tidak akan menerima pemberitahuan lagi saat terjadi gerakan", "");
    }


    
  if (text == "/AlamatIP"){
     
      bot.sendMessage(chat_id, "http://192.168.43.66/mjpeg/1", "");//wajib ganti ip
      
    }
     if (text == "/status")
    {
      if (flag)
      {
        bot.sendMessage(chat_id, "Sensor Sudah Hidup", "");
      }
      else
      {
        bot.sendMessage(chat_id, "Sensor Sudah mati", "");
      }
    }        
  }
}

String sendPhotoTelegram() {
  const char* myDomain = "api.telegram.org";
  String getAll = "";
  String getBody = "";

  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();  
  if(!fb) {
    Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
    return "Camera capture failed";
  }  
  
  Serial.println("Connect to " + String(myDomain));


  if (clientTCP.connect(myDomain, 443)) {
    Serial.println("Connection successful");
    
    String head = "--RandomNerdTutorials\r\nContent-Disposition: form-data; name=\"chat_id\"; \r\n\r\n" + CHAT_ID + "\r\n--RandomNerdTutorials\r\nContent-Disposition: form-data; name=\"photo\"; filename=\"esp32-cam.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
    String tail = "\r\n--RandomNerdTutorials--\r\n";

    uint16_t imageLen = fb->len;
    uint16_t extraLen = head.length() + tail.length();
    uint16_t totalLen = imageLen + extraLen;
  
    clientTCP.println("POST /bot"+BOTtoken+"/sendPhoto HTTP/1.1");
    clientTCP.println("Host: " + String(myDomain));
    clientTCP.println("Content-Length: " + String(totalLen));
    clientTCP.println("Content-Type: multipart/form-data; boundary=RandomNerdTutorials");
    clientTCP.println();
    clientTCP.print(head);
  
    uint8_t *fbBuf = fb->buf;
    size_t fbLen = fb->len;
    for (size_t n=0;n<fbLen;n=n+1024) {
      if (n+1024<fbLen) {
        clientTCP.write(fbBuf, 1024);
        fbBuf += 1024;
      }
      else if (fbLen%1024>0) {
        size_t remainder = fbLen%1024;
        clientTCP.write(fbBuf, remainder);
      }
    }  
    
    clientTCP.print(tail);
    
    esp_camera_fb_return(fb);
    
    int waitTime = 10000;   // timeout 10 seconds
    long startTimer = millis();
    boolean state = false;
    
    while ((startTimer + waitTime) > millis()){
      Serial.print(".");
      delay(100);      
      while (clientTCP.available()) {
        char c = clientTCP.read();
        if (state==true) getBody += String(c);        
        if (c == '\n') {
          if (getAll.length()==0) state=true; 
          getAll = "";
        } 
        else if (c != '\r')
          getAll += String(c);
        startTimer = millis();
      }
      if (getBody.length()>0) break;
    }
    clientTCP.stop();
    Serial.println(getBody);
  }
  else {
    getBody="Connected to api.telegram.org failed.";
    Serial.println("Connected to api.telegram.org failed.");
  }
  return getBody;
}

void setup(){
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  // Init Serial Monitor
  Serial.begin(115200);
 // pinMode(led,OUTPUT);
  pinMode(led_dan_buzzer,OUTPUT);
  // Set LED Flash as output
  pinMode(FLASH_LED_PIN, OUTPUT);
  pinMode(sensor1, INPUT);
  
  digitalWrite(FLASH_LED_PIN, flashState);

  // Config and init the camera
  configInitCamera();

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  clientTCP.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println(F("WiFi connected"));
  Serial.println("");
  Serial.println(WiFi.localIP());
  Serial.print("Stream Link: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/mjpeg/1");
   String AWAL = "SELAMAT DATANG DI SYSTEM\n";
      AWAL += "KEAMANAN RUMAH BERBASIS IoT \n";
      AWAL += "Silangkan klik /start \n";
      AWAL += "TERIMAKASIH  \n";
      bot.sendMessage(CHAT_ID, AWAL, "");
  server.on("/mjpeg/1", HTTP_GET, handle_jpg_stream);
  server.on("/jpg", HTTP_GET, handle_jpg);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  server.handleClient();
  if (sendPhoto) {
    Serial.println("Preparing photo");
    sendPhotoTelegram(); 
    sendPhoto = false; 
  }

  if (flag){
   // delay(1000);
    
    int kondisiSensor = digitalRead(sensor1);
    if(kondisiSensor==LOW){
       digitalWrite(led_dan_buzzer,HIGH);//1
      delay(waktu1);
       digitalWrite(led_dan_buzzer,LOW);
      delay(waktu0);
      Serial.print("Motion Detected, Value = ");
      Serial.println(digitalRead(sensor1));
      String motion = "Terdeteksi gerakan!!\n";
      motion += "Harap Tunggu Foto akan segera dikirim\n";
      bot.sendMessage(CHAT_ID, motion, "");
      sendPhotoTelegram();
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
              digitalWrite(led_dan_buzzer,HIGH);//11
      delay(waktu0);
       digitalWrite(led_dan_buzzer,LOW);
       delay(waktu0);
              digitalWrite(led_dan_buzzer,HIGH);//12
       delay(waktu1);
       digitalWrite(led_dan_buzzer,LOW);
        delay(waktu0);
              digitalWrite(led_dan_buzzer,HIGH);//13
       delay(waktu1);
       digitalWrite(led_dan_buzzer,LOW);
       delay(waktu0);
              digitalWrite(led_dan_buzzer,HIGH);//14
     delay(waktu1);
       digitalWrite(led_dan_buzzer,LOW);
       delay(waktu0);
              digitalWrite(led_dan_buzzer,HIGH);//15
       delay(waktu1);
       digitalWrite(led_dan_buzzer,LOW);
       delay(waktu0);
    } 
  }
  
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
