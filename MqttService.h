#pragma once
#include <WiFi.h>
#include <PubSubClient.h>

class MqttService {
  private:
    WiFiClient _wifiClient;
    PubSubClient _client;
    const char* _brokerIp = "192.168.0.104"; // Tu IP de Manjaro
    unsigned long _lastReconnectAttempt = 0;

  public:
    MqttService() : _client(_wifiClient) {}

    void begin() {
        _client.setServer(_brokerIp, 1883);
        _client.setCallback([this](char* topic, byte* payload, unsigned int length) {
            this->callback(topic, payload, length);
        });
    }

    void handle() {
        if (!_client.connected()) {
            unsigned long now = millis();
            // Solo intenta reconectar cada 5 segundos, sin detener el loop
            if (now - _lastReconnectAttempt > 5000) {
                _lastReconnectAttempt = now;
                if (reconnect()) {
                    _lastReconnectAttempt = 0;
                }
            }
        } else {
            _client.loop();
        }
    }

  private:
    bool reconnect() {
        Serial.print(F("[MQTT] Intentando conexión..."));
        if (_client.connect("ESP32_Client")) {
            Serial.println(F("conectado"));
            _client.subscribe("casa/rele1");
            return true;
        }
        Serial.print(F("falló, rc="));
        Serial.println(_client.state());
        return false;
    }

    void callback(char* topic, byte* payload, unsigned int length) {
        String message;
        for (int i = 0; i < length; i++) message += (char)payload[i];
        
        Serial.printf("\n[MQTT-TEST] Tópico: %s | Mensaje: %s\n", topic, message.c_str());

        if (String(topic) == "casa/rele1") {
            Serial.println(message == "ON" ? ">> VIRTUAL RELE 1: ACTIVADO <<" : ">> VIRTUAL RELE 1: DESACTIVADO <<");
        }
        if (String(topic) == "casa/rele2") {
            Serial.println(message == "ON" ? ">> VIRTUAL RELE 2: ACTIVADO <<" : ">> VIRTUAL RELE 2: DESACTIVADO <<");
        }
    }
};