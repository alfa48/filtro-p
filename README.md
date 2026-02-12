# 🟢 filtro-p

Sistema IoT para controlo remoto de um filtro/motor (ex: purificador de água) usando **ESP32 + MQTT + Web Interface**.

O projeto permite **ligar e desligar o motor em tempo real**, com sincronização de estado entre o hardware (ESP32) e a interface web através de um **broker MQTT público**.

---

## 🚀 Funcionalidades

- ✅ Controlo ON / OFF do motor ou LED
- 🌐 Interface Web em tempo real
- 🔌 Comunicação via **MQTT (WebSocket)**
- 🔁 Sincronização de estado (cmd / state)
- 🧠 UI reflete o estado real do dispositivo
- ♻️ Reconexão automática Wi-Fi e MQTT
- 📡 Compatível com ESP32 (Wokwi ou físico)

---

## 🧱 Arquitetura

```text
[ Web Browser ]
      |
      |  MQTT (WebSocket)
      v
[ Broker MQTT ]
      ^
      |  MQTT (TCP)
      |
[ ESP32 ]
