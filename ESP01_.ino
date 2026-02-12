#include <WiFi.h>
#include <PubSubClient.h>

// =====================
// Credenciais Wi-Fi
// =====================
const char* ssid = "Wokwi-GUEST";   // substitua pela rede real no ESP físico
const char* password = ""; // senha da rede wifi local com acesso a internet

// =====================
// Configuração MQTT
// =====================
const char* mqttServer = "broker.hivemq.com";
//const char* mqttServer = "test.mosquitto.org"; // broker público TCP
const int mqttPort = 1883;                      // porta MQTT padrão
const char* mqttUser = "";//"meuUsuario";           // mesmo que no HTML
const char* mqttPassword = "";//"minhaSenha123";    // mesmo que no HTML

// =====================
// Tópicos MQTT
// =====================
const char* topicCmd   = "LEDE/cmd";
const char* topicState = "LEDE/state";
const char* topicLED = "LEDE";  // mesmo que no HTML

// =====================
// Pinos e variáveis
// =====================
#define LED 2      // LED embutido
int ValueToSend = 0;

// =====================
// Objetos MQTT e WiFi
// =====================
WiFiClient espClient;
PubSubClient client(espClient);

// =====================
// Função de callback MQTT
// =====================
void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.print("Mensagem recebida no tópico ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(msg);

  // Comando LED
  if (String(topic) == topicCmd) {
    if (msg == "ON") {
      digitalWrite(LED, HIGH);
      Serial.println("LED ligado!");
      client.publish(topicState, "ON", true); // retain
    } else if (msg == "OFF") {
      digitalWrite(LED, LOW);
      Serial.println("LED desligado!");
      client.publish(topicState, "OFF", true);
    }
  }
}

// =====================
// Reconectar MQTT
// =====================
void reconnectMQTT() {
  String clientId = "ESP32Client_" + String(random(0xffff), HEX);

  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
      Serial.println("conectado!");
      client.subscribe(topicCmd); // assina tópico LED
    } else {
      Serial.print("Falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando de novo em 2s");
      delay(2000);
    }
  }
}

// =====================
// Reconectar Wi-Fi
// =====================
void reconnectWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconectando Wi-Fi...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    delay(2000);
  }
  Serial.println("Wi-Fi reconectado!");
}

// =====================
// Setup
// =====================
void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);


  // Conectar Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }
  Serial.println("Wi-Fi conectado!");

  // Configurar MQTT
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

// =====================
// Loop principal
// =====================
void loop() {
  // Reconectar Wi-Fi se cair
  if (WiFi.status() != WL_CONNECTED) {
    reconnectWiFi();
  }

  // Reconectar MQTT se cair
  if (!client.connected()) {
    reconnectMQTT();
  }

  client.loop();

  delay(200); // intervalo de envio
}
