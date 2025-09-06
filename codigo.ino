#include <ArduinoJson.h>

const int ledPin = 7 ;    // LED indicador
const int sensorPin = 2;  // Pino do sensor de gol (ex.: sensor IR ou botão)
int totalGols = 0;        // contador total de gols

StaticJsonDocument<64> doc;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT); 
  Serial.begin(9600);
}

void loop() {
  // Checa se o sensor detectou gol
  if (digitalRead(sensorPin) == LOW) {  // LOW quando o sensor ativa
    totalGols++;
    
    // Envia JSON com a variável 'gols' para o Node-RED
    doc["gols"] = totalGols;
    serializeJson(doc, Serial);
    Serial.println();
    
    // Pisca LED p
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    
    // Evita múltiplas contagens pelo mesmo gol 
    while(digitalRead(sensorPin) == LOW) {
      delay(10);
    }
  }
}
