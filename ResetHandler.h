#pragma once
#include <Arduino.h>

class ResetHandler {
  private:
    int _pin;
    void (*_callback)();
    unsigned long _duration;
    unsigned long _startTime;
    bool _isPressing;

  public:
    ResetHandler(int pin, void (*callback)(), unsigned long duration = 4000)
        : _pin(pin), _callback(callback), _duration(duration), _startTime(0), _isPressing(false) {}

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
                if (_callback) _callback();
            }
        } else {
            _isPressing = false;
        }
    }
};
