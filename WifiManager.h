#pragma once
#include <WiFi.h>
#include <Preferences.h>
#include "BluetoothSerial.h"

class WifiManager {
  private:
    BluetoothSerial SerialBT;
    Preferences pref;
    String _ssid;
    String _pass;

  public:
    void begin() {
        WiFi.mode(WIFI_STA);
        
        pref.begin("wifi-conf", true);
        _ssid = pref.getString("ssid", "");
        _pass = pref.getString("pass", "");
        pref.end();

        if (_ssid != "" && _ssid != "NULL") {
            Serial.printf("Trying to connect to: %s\n", _ssid.c_str());
            WiFi.begin(_ssid.c_str(), _pass.c_str());

            uint8_t count = 0;
            while (WiFi.status() != WL_CONNECTED && count < 20) {
                delay(500);
                Serial.print(".");
                count++;
            }

            if (WiFi.status() == WL_CONNECTED) {
                Serial.println("\nSuccessful connection!");
                Serial.print("IP: "); Serial.println(WiFi.localIP());
                return;
            }
        }
        
        Serial.println("\nCould not connect. Starting Bluetooth...");
        configViaBluetooth();
    }

    void configViaBluetooth() {
        SerialBT.begin("ESP32_Config");
        Serial.println("Send: SSID,PASSWORD");

        while (true) {
            if (SerialBT.available()) {
                String credientials = SerialBT.readStringUntil('\n');
                credientials.trim();
                int comaIndex = credientials.indexOf(',');

                if (comaIndex != -1) {
                    _ssid = credientials.substring(0, comaIndex);
                    _pass = credientials.substring(comaIndex + 1);

                    Serial.printf("Received via BT. SSID: %s\n", _ssid.c_str());
                    WiFi.begin(_ssid.c_str(), _pass.c_str());

                    if (WiFi.waitForConnectResult() == WL_CONNECTED) {
                        pref.begin("wifi-conf", false);
                        pref.putString("ssid", _ssid);
                        pref.putString("pass", _pass);
                        pref.end();

                        String message = "Success. IP: " + WiFi.localIP().toString();
                        SerialBT.println(message);
                        Serial.println(message);
                        break;
                    } else {
                        SerialBT.println("Error: Could not connect to that network. Retry.");
                    }
                }
            }
            delay(100);
        }
        SerialBT.end();
    }

    void resetSettings() {
        Serial.println("[WiFi] !!! DELETING CONFIGURATION !!!");
        pref.begin("wifi-conf", false);
        pref.clear();
        pref.end();
        Serial.println("[WiFi] CONFIGURATION DELETED");
    }
};
