#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
const char* WIFI_SSID="YOUR_WIFI"; const char* WIFI_PASSWORD="YOUR_PASSWORD";
const char* MQTT_HOST="192.168.1.10"; const int MQTT_PORT=1883;
const char* GATE="exit"; const char* DEVICE="ESP32_EXIT";
#define RFID_SS 5
#define RFID_RST 22
#define SERVO_PIN 13
#define BUZZER_PIN 27
WiFiClient net; PubSubClient mqtt(net); MFRC522 reader(RFID_SS,RFID_RST); Servo servo;
String tCmd,tStatus,tRfid;
void statusMsg(const char* state,const char* result){String s="{\"gate_id\":\""+String(GATE)+"\",\"device_id\":\""+String(DEVICE)+"\",\"state\":\""+String(state)+"\",\"result\":\""+String(result)+"\"}";mqtt.publish(tStatus.c_str(),s.c_str(),false);}
void onMqtt(char* topic,byte* payload,unsigned int len){String s;for(unsigned int i=0;i<len;i++)s+=(char)payload[i];if(String(topic)!=tCmd)return;if(s.indexOf("\"command\":\"OPEN\"")>=0){servo.write(90);delay(700);statusMsg("open","EXECUTED");}else if(s.indexOf("\"command\":\"CLOSE\"")>=0){servo.write(0);delay(700);statusMsg("closed","EXECUTED");}}
void connectMqtt(){while(!mqtt.connected()){String id=String(DEVICE)+"-"+String((uint32_t)ESP.getEfuseMac(),HEX);if(mqtt.connect(id.c_str())){mqtt.subscribe(tCmd.c_str(),1);statusMsg("closed","ONLINE");}else delay(2000);}}
void setup(){Serial.begin(115200);pinMode(BUZZER_PIN,OUTPUT);servo.setPeriodHertz(50);servo.attach(SERVO_PIN,500,2400);servo.write(0);SPI.begin();reader.PCD_Init();WiFi.begin(WIFI_SSID,WIFI_PASSWORD);while(WiFi.status()!=WL_CONNECTED)delay(500);tCmd="parking/"+String(GATE)+"/command";tStatus="parking/"+String(GATE)+"/status";tRfid="parking/"+String(GATE)+"/rfid";mqtt.setServer(MQTT_HOST,MQTT_PORT);mqtt.setCallback(onMqtt);}
void loop(){if(!mqtt.connected())connectMqtt();mqtt.loop();if(!reader.PICC_IsNewCardPresent()||!reader.PICC_ReadCardSerial())return;String uid;for(byte i=0;i<reader.uid.size;i++){if(reader.uid.uidByte[i]<16)uid+="0";uid+=String(reader.uid.uidByte[i],HEX);}uid.toUpperCase();String p="{\"uid\":\""+uid+"\",\"gate_id\":\""+String(GATE)+"\",\"device_id\":\""+String(DEVICE)+"\"}";mqtt.publish(tRfid.c_str(),p.c_str(),false);tone(BUZZER_PIN,1800,100);reader.PICC_HaltA();reader.PCD_StopCrypto1();delay(1200);}
