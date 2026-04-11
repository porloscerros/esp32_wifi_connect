#include <Arduino.h>
#include "WifiManager.h"
#include "ResetHandler.h"

WifiManager myWifi;

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
    myWifi.begin();
}

void loop() {
    systemReset.check();
}
