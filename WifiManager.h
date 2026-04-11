#pragma once
#include <WiFi.h>
#include <Preferences.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>

class WifiManager {
  private:
    BluetoothSerial SerialBT;
    Preferences pref;

  public:
    void begin() {
        pref.begin("wifi-conf", true);
        String ssid = pref.getString("ssid", "");
        String pass = pref.getString("pass", "");
        pref.end();

        if (ssid != "") {
            Serial.print("[WiFi] Conectando a: "); Serial.println(ssid);
            WiFi.begin(ssid.c_str(), pass.c_str());
            int retry = 0;
            while (WiFi.status() != WL_CONNECTED && retry < 20) {
                delay(500); Serial.print("."); retry++;
            }
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\n[WiFi] WiFi Conectado");
            Serial.print("[WiFi] IP: "); Serial.println(WiFi.localIP());
            return;
        }

        configBT();
    }

    void configBT() {
        if (!SerialBT.hasClient()) SerialBT.begin("ESP32_Config_App");
        Serial.println("\n[WiFi] Modo Configuración BT Activo. Conectate con 'ESP32_Config_App'");

        while (WiFi.status() != WL_CONNECTED) {
            if (SerialBT.available()) {
                String input = SerialBT.readStringUntil('\n'); 
                input.trim();
                if (input.length() > 0) {
                    StaticJsonDocument<256> doc;
                    DeserializationError error = deserializeJson(doc, input);
                    if (error) {
                        SerialBT.println("{\"status\":\"error\", \"msg\":\"JSON mal formado: " + String(error.c_str()) + "\"}");
                        continue; 
                    }
                    if (!doc.containsKey("type") || doc["type"].isNull()) {
                        SerialBT.println("{\"status\":\"error\", \"msg\":\"Falta el campo type\"}");
                        continue;
                    }

                    if (doc["type"] == "scan") {
                        String jsonResponse = getScanResultsJSON();
                        SerialBT.println(jsonResponse);
                    }
                    else if (doc["type"] == "wifi") {
                        String s = doc["ssid"];
                        String p = doc["pass"];
                        WiFi.disconnect(true);
                        delay(100); 
                        Serial.print("[WiFi] Conectando a: "); Serial.println(s);
                        WiFi.begin(s.c_str(), p.c_str());
                        if (WiFi.waitForConnectResult() == WL_CONNECTED) {
                            pref.begin("wifi-conf", false);
                            pref.putString("ssid", s);
                            pref.putString("pass", p);
                            pref.end();
                            
                            String localIP = WiFi.localIP().toString();
                            Serial.println("\n[WiFi] WiFi Conectado. IP: " + localIP);
                            SerialBT.println("{ \"status\": \"ok\", \"msg\": \"WiFi Conectado\", \"ip\": \"" + localIP + "\" }");
                            delay(500);
                            SerialBT.end();
                            break;
                        }
                        else {
                            Serial.println("[WiFi] Fallo de conexión a la red");
                            SerialBT.println("{\"status\":\"error\", \"msg\":\"Fallo de conexión a la red\"}");
                        }
                    }
                    else {
                        SerialBT.println("{\"status\":\"error\", \"msg\":\"Tipo no valido\"}");
                    }
                }
            }
            delay(100);
        }
    }

    String getScanResultsJSON() {
        int n = WiFi.scanNetworks();
        StaticJsonDocument<768> doc;
        doc["type"] = "scan_res";
        JsonArray nets = doc.createNestedArray("networks");
        int limit = (n > 10) ? 10 : n;
        for (int i = 0; i < limit; i++) {
            nets.add(WiFi.SSID(i));
        }
        String out;
        serializeJson(doc, out);
        return out;
    }

    void clearCredentials() {
        pref.begin("wifi-conf", false);
        pref.clear();
        pref.end();
    }

};
