#include "WifiManager.h"
#include "ResetHandler.h"

WifiManager myWifi;

void globalReset() {
    myWifi.resetSettings();
    // mySensor.resetCalibration();
    // myCounter.clearHistory();
}

ResetHandler systemReset(0, globalReset, 4000);

void setup() {
    Serial.begin(115200);
    systemReset.begin();
    myWifi.begin();
}

void loop() {
    systemReset.check();
}
