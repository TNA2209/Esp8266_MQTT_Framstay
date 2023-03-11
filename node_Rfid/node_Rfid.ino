#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "SPI.h"
#include "MFRC522.h"
#include <Servo.h>
#include <ArduinoJson.h>

#define MQTT_VERSION MQTT_VERSION_3_1_1
#define SS_PIN 4
#define RST_PIN 5
#define BUZZER_PIN 0
// Wifi: SSID and password
const PROGMEM char* WIFI_SSID = "farmstay";
const PROGMEM char* WIFI_PASSWORD = "16122209";

// MQTT: ID, server IP, port, username and password
const PROGMEM char* MQTT_CLIENT_ID = "rfid";
const PROGMEM char* MQTT_SERVER_IP = "134.122.6.117";
const PROGMEM uint16_t MQTT_SERVER_PORT = 1883;
const PROGMEM char* MQTT_USER = "farmstay";
const PROGMEM char* MQTT_PASSWORD = "farmstay";
char* MQTT_SENSOR_TOPIC1 = "efbd40e7-c502-4793-9ffa-283973bbbf53/rfid-rc522";
String strID="";
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 5000; // send data every 10 seconds
Servo myServo;

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

WiFiClient wifiClient;
PubSubClient client(wifiClient);


// function called when a MQTT message arrived
void callback(char* p_topic, byte* p_payload, unsigned int p_length) {
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(MQTT_CLIENT_ID,MQTT_USER,MQTT_PASSWORD)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void setup_wifi()
{
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}
String rfidCard(){
  strID="";
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
  return "";
  for (byte i = 0; i < 4; i++) {
    strID += (rfid.uid.uidByte[i] < 0x10 ? "0" : "") + String(rfid.uid.uidByte[i], HEX) + (i!=3 ? ":" : "");
  }
  strID.toUpperCase();
return strID;
}
void setup() {
  // init the serial
  Serial.begin(115200);
  setup_wifi();
  SPI.begin();
  myServo.attach(16); 
  myServo.write(0);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);
  rfid.PCD_Init();
  // init the MQTT connection
  client.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); 
  String UID1= "D3:75:75:08";
  String UID2= "F3:EA:3A:08";
  String UID3 ="63:26:9B:08";
  String UID4 ="F3:08:86:A3";
  
  if (client.connected()){
    String rfidCardValue = rfidCard();
    if (rfidCardValue != ""){
      if (millis() - lastSendTime >= sendInterval) { // check if it's time to send data
        Serial.println(rfidCardValue);
        if (rfidCardValue == UID1){
          StaticJsonBuffer<200> jsonBuffer;
          JsonObject& root = jsonBuffer.createObject();
          root["id"] = "Tran Nam Anh";
          root["uid"] = UID1;
          root.prettyPrintTo(Serial);
          Serial.println("");
          /*
          {
            "id": "Tran Nam Anh",
            "uid":  "D3:75:75:08"
          }
          */
          char data[200];
          root.printTo(data, root.measureLength() + 1);
          client.publish(MQTT_SENSOR_TOPIC1, data);
          myServo.write(90);
          Serial.println("Open door");
          digitalWrite(BUZZER_PIN, LOW); // turn on the buzzer
          delay(1000); // wait for 1 seconds
          digitalWrite(BUZZER_PIN, HIGH); // turn off the buzzer
          delay(4000); // wait for 4 seconds
          myServo.write(0);
          Serial.println("Close door");
        }
        else if (rfidCardValue == UID2){
          StaticJsonBuffer<200> jsonBuffer;
          JsonObject& root = jsonBuffer.createObject();
          root["id"] = "Tang Ho Trung Nam";
          root["uid"] = UID2;
          root.prettyPrintTo(Serial);
          Serial.println("");
          /*
          {
            "id": "Tang Ho Trung Nam",
            "uid":  "F3:EA:3A:08"
          }
          */
          char data[200];
          root.printTo(data, root.measureLength() + 1);
          client.publish(MQTT_SENSOR_TOPIC1, data);
          myServo.write(90);
          Serial.println("Open door");
          digitalWrite(BUZZER_PIN, LOW); // turn on the buzzer
          delay(1000); // wait for 1 seconds
          digitalWrite(BUZZER_PIN, HIGH); // turn off the buzzer
          delay(4000); // wait for 4 seconds
          myServo.write(0);
          Serial.println("Close door");
        }
        else if (rfidCardValue == UID3){
          StaticJsonBuffer<200> jsonBuffer;
          JsonObject& root = jsonBuffer.createObject();
          root["id"] = "Doan Hoang Ca";
          root["uid"] = UID3;
          root.prettyPrintTo(Serial);
          Serial.println("");
          /*
          {
            "id": "Doan Hoang Ca",
            "uid":  "63:26:9B:08"
          }
          */
          char data[200];
          root.printTo(data, root.measureLength() + 1);
          client.publish(MQTT_SENSOR_TOPIC1, data);
          myServo.write(90);
          Serial.println("Open door");
          digitalWrite(BUZZER_PIN, LOW); // turn on the buzzer
          delay(1000); // wait for 1 seconds
          digitalWrite(BUZZER_PIN, HIGH); // turn off the buzzer
          delay(4000); // wait for 4 seconds
          myServo.write(0);
          Serial.println("Close door");
        }
         else if (rfidCardValue == UID4){
          StaticJsonBuffer<200> jsonBuffer;
          JsonObject& root = jsonBuffer.createObject();
          root["id"] = "custommer";
          root["uid"] = UID4;
          root.prettyPrintTo(Serial);
          Serial.println("");
          /*
          {
            "id": "custommer",
            "uid":  "F3:08:86:A3"
          }
          */
          char data[200];
          root.printTo(data, root.measureLength() + 1);
          client.publish(MQTT_SENSOR_TOPIC1, data);
          myServo.write(90);
          Serial.println("Open door");
          digitalWrite(BUZZER_PIN, LOW); // turn on the buzzer
          delay(1000); // wait for 1 seconds
          digitalWrite(BUZZER_PIN, HIGH); // turn off the buzzer
          delay(4000); // wait for 4 seconds
          myServo.write(0);
          Serial.println("Close door");
        }
        else{
          Serial.println("No Information");
        }
        lastSendTime = millis(); // update the last send time
      }
    }   
  } 
}
