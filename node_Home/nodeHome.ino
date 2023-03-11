#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define channel1_pin D0 
#define channel2_pin D1  
#define channel3_pin D2
#define channel4_pin D3  
#define FirePin D4
#define waterLevelPin  A0

// Wifi: SSID and password
const char *WIFI_SSID = "farmstay";
const char *WIFI_PASSWORD = "16122209";

// MQTT: ID, server IP, port, username and password
const char* MQTT_CLIENT_ID = "home";
const char *MQTT_SERVER_IP = "134.122.6.117";
const uint16_t MQTT_SERVER_PORT = 1883;
const char *MQTT_USER = "farmstay";
const char *MQTT_PASSWORD = "farmstay";

// MQTT: topic
const char *channel1_topic = "relay1";
const char *channel2_topic = "relay2";
const char *channel3_topic = "relay3";
const char *channel4_topic = "relay4";
const char *MQTT_SENSOR_TOPIC1 = "efbd40e7-c502-4793-9ffa-283973bbbf53/cam-bien-lua";
const char *MQTT_SENSOR_TOPIC2 = "efbd40e7-c502-4793-9ffa-283973bbbf53/cam-bien-muc-nuoc";


// Define variables to keep track of the last time sensor data and relay control were sent
const unsigned long sensorInterval = 5000; // Send sensor data every 5 seconds
unsigned long lastSensorTime = 0;


WiFiClient espClient;
PubSubClient client(espClient);

// Publish data sensor fire
void publishData(int p_fire) {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["id"] = "gz050p7arfjchwut";
  root["value"] = (int)p_fire;
  root.prettyPrintTo(Serial);
  Serial.println("");
  /*
  {
    "id": "gz050p7arfjchwut",
    "value": 0
  }
  */
  char data[200];
  root.printTo(data, root.measureLength() + 1);
  client.publish(MQTT_SENSOR_TOPIC1, data);
}

// Publish data sensor waterlevel
void publishData1(int p_waterlevel) {
  
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["id"] = "1hyf2swyd4zfvzvs";
  root["value"] = (int)p_waterlevel;
  root.prettyPrintTo(Serial);
  Serial.println("");
  /*
  {
    "id": "1hyf2swyd4zfvzvs",
    "value": 0
  }
  */
  char data[200];
  root.printTo(data, root.measureLength() + 1);
  client.publish(MQTT_SENSOR_TOPIC2, data);
}

void setup_wifi()
{
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("INFO: Connecting to ");
    Serial.println(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("INFO: WiFi connected");
    Serial.println("INFO: IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{
    // Handle messages on subscribed topics
    if (strcmp(topic, channel1_topic) == 0)
    {
        if (payload[0] == '1')
        {
            digitalWrite(channel1_pin, LOW);
        }
        else
        {
            digitalWrite(channel1_pin, HIGH);
        }
    }
    if (strcmp(topic, channel2_topic) == 0)
    {
        if (payload[0] == '1')
        {
            digitalWrite(channel2_pin, LOW);
        }
        else
        {
            digitalWrite(channel2_pin, HIGH);
        }
    }
    if (strcmp(topic, channel3_topic) == 0)
    {
        if (payload[0] == '1')
        {
            digitalWrite(channel3_pin, LOW);
        }
        else
        {
            digitalWrite(channel3_pin, HIGH);
        }
    }
    if (strcmp(topic, channel4_topic) == 0)
    {
        if (payload[0] == '1')
        {
            digitalWrite(channel4_pin, LOW);
        }
        else
        {
            digitalWrite(channel4_pin, HIGH);
        }
    }
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("INFO: Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD))
        {
            Serial.println("INFO: connected");
            // Once connected, subscribe to the channels for each relay
            client.subscribe(channel1_topic);
            client.subscribe(channel2_topic);
            client.subscribe(channel3_topic);
            client.subscribe(channel4_topic);
        }
        else
        {
            Serial.print("ERROR: failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void setup()
{
    Serial.begin(115200);
    setup_wifi();
    client.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
    client.setCallback(callback);
    // Set Relay pins as outputs
    pinMode(channel1_pin, OUTPUT);
    pinMode(channel2_pin, OUTPUT);
    pinMode(channel3_pin, OUTPUT);
    pinMode(channel4_pin, OUTPUT);
    digitalWrite(channel1_pin, HIGH);
    digitalWrite(channel2_pin, HIGH);
    digitalWrite(channel3_pin, HIGH);
    digitalWrite(channel4_pin, HIGH);
    pinMode(waterLevelPin, INPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Get the current time
  unsigned long currentTime = millis();

  // Check if it's time to send sensor data
  if (currentTime - lastSensorTime >= sensorInterval) {
    int flameValue = digitalRead(FirePin);
    if (digitalRead(FirePin) == LOW) {
    flameValue = 1; // Có lửa
  } else {
    flameValue = 0; // Không có lửa
  }
    publishData(flameValue);
    int waterLevel = analogRead(waterLevelPin);
    publishData1(waterLevel);
    lastSensorTime = currentTime; // Update the last sensor time
  }
}
