#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// WiFi & MQTT Config
const char* ssid = "ZenIndahhp";
const char* pass = "ZenIndah";
const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(5, DHT11);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  dht.begin();
  WiFi.begin(ssid, pass);
  WiFi.setTxPower(WIFI_POWER_8_5dBm); 
  while (WiFi.status() != WL_CONNECTED) delay(500);
  
  client.setServer(mqtt_server, 1883); // Port standar MQTT
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32_Zen_Client")) {
      Serial.println("MQTT Connected");
    } else {
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  static unsigned long lastMsg = 0;
  if (millis() - lastMsg > 2000) {
    lastMsg = millis();
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    
    if (!isnan(t) && !isnan(h)) {
      client.publish("zen_indah/suhu", String(t).c_str());
      client.publish("zen_indah/hum", String(h).c_str());
      Serial.println("Data Terkirim!");
    }
  }
}
