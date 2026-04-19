#include "WifiManager.h"
#include "MqttService.h"
#include "ResetHandler.h"

WifiManager myWifi;
MqttService mqtt;

void globalReset() {
    Serial.println("\n[SYSTEM] RESET DE FÁBRICA...");
    myWifi.clearCredentials();
    delay(1000);
    ESP.restart();
}

ResetHandler systemReset(0, globalReset, 4000);

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("--- DISPOSITIVO IOT INICIADO ---");
    systemReset.begin();
    if (digitalRead(0) == LOW) {
        globalReset();
    }
    myWifi.begin(&systemReset);
    mqtt.begin();
}

void loop() {
    systemReset.check();
    if (WiFi.status() == WL_CONNECTED) {
        mqtt.handle();
    }
}
