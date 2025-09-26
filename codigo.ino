/*
  Projeto IoT - Contagem de Gols via IR (Wokwi)
  Integrantes:
  João Vitor Parizotto Rocha | RM: 562719
  Giovana Bernardino Carnevali | RM: 566196
  Alexandre Freitas Silva | RM: 566278
  Felipe Rodrigues Gomes Ribeiro | RM: 562482
  Artur Distrutti Santos | RM: 561319
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// ======== Config Wi-Fi =========
const char* ssid = "Wokwi-GUEST";   // Wi-Fi da simulação Wokwi
const char* password = "";

// ======== Config MQTT ==========
const char* mqtt_server = "54.173.242.135";
const int mqtt_port = 1883;
const char* topic = "inova/futebol/gols";

// ======== Pinos ESP32 =========
const int ledPin = 18;
const int sensorIR = 2;

int totalGols = 0;

// ======== Objetos ==========
WiFiClient espClient;
PubSubClient client(espClient);
StaticJsonDocument<64> doc;

// ======== Funções ==========
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(sensorIR, INPUT);

  Serial.begin(115200);

  // Conectar Wi-Fi
  Serial.print("Conectando WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" conectado!");

  // Conectar MQTT
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Simula gol com sensor IR
  if (digitalRead(sensorIR) == LOW) {
    totalGols++;

    // JSON {"gols": X}
    doc["gols"] = totalGols;
    char buffer[64];
    serializeJson(doc, buffer);

    // Publicar no broker público
    client.publish(topic, buffer);

    Serial.print("Gol detectado! Total = ");
    Serial.println(totalGols);

    // Feedback LED
    digitalWrite(ledPin, HIGH);
    delay(300);
    digitalWrite(ledPin, LOW);

    // Debounce simples
    while (digitalRead(sensorIR) == LOW) {
      delay(10);
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando MQTT...");
    if (client.connect("ESP32-Gols")) {
      Serial.println(" conectado ao broker!");
    } else {
      Serial.print(" falhou, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}
