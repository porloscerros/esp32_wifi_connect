#pragma once
#include <Arduino.h>

class ResetHandler {
  private:
    int _pin;
    unsigned long _duration;
    unsigned long _startTime;
    bool _isPressing;
    void (*_onResetCallback)();

  public:
    ResetHandler(int pin, void (*callback)(), unsigned long duration = 3000) 
        : _pin(pin), _onResetCallback(callback), _duration(duration), _startTime(0), _isPressing(false) {}

    void begin() {
        pinMode(_pin, INPUT_PULLUP);
    }

    void check() {
        if (digitalRead(_pin) == LOW) {
            if (!_isPressing) {
                _startTime = millis();
                _isPressing = true;
            }

            if (millis() - _startTime > _duration) {
                Serial.println("[System] RESET");
                if (_onResetCallback) _onResetCallback();
                Serial.println("[System] REBOOTING...");
                delay(1000);
                ESP.restart();
            }
        } else {
            _isPressing = false;
        }
    }
};
