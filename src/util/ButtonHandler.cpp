#include "Arduino.h"
#include "ButtonHandler.h"

ButtonHandler::ButtonHandler(
    uint8_t buttonPin,
    void (*onPress)(),
    void (*onRelease)()
) {
    _buttonPin = buttonPin;
    _onPress = onPress;
    _onRelease = onRelease;
    
    lastPhysicalState = false;
    currentLogicalState = false;
}

void ButtonHandler::init() {
    pinMode(_buttonPin, INPUT);
}

void ButtonHandler::update() {
    unsigned long now = millis();
    if (now - _lastLogicalStateChangeMs < LOCKOUT_MS) return;
    
    bool currentPhysicalState = analogRead(_buttonPin) >= VOLTAGE_THRESHOLD;
    
    if (
        // ...button's pressed when it wasn't, or the contrary...
        (currentLogicalState != currentPhysicalState) &&
        // ...once again...
        (currentPhysicalState == lastPhysicalState)
    ) { // ...steadily, for the last [threshold]ms
        if (currentPhysicalState && (now - _lastPhysicalStateChangeMs >= HIGH_MS_THRESHOLD)) {
            _lastLogicalStateChangeMs = now;
            currentLogicalState = currentPhysicalState;
            if (_onPress) _onPress();
        } else if (!currentPhysicalState && (now - _lastPhysicalStateChangeMs >= LOW_MS_THRESHOLD)) {
            _lastLogicalStateChangeMs = now;
            currentLogicalState = currentPhysicalState;
            if (_onRelease) _onRelease();
        } else {
            // (still pressed, but we're waiting for the threshold)
            // the rest can be skipped
            return;
        }
    }
    
    
    if (currentPhysicalState != lastPhysicalState) _lastPhysicalStateChangeMs = now;
    lastPhysicalState = currentPhysicalState;
}