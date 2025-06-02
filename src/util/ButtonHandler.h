#pragma once

#include "Arduino.h"

// Will engage if the signal persists [x]ms after first detection
#define HIGH_MS_THRESHOLD 20

// Will disengage if the signal's gone for [x]ms after first detection
#define LOW_MS_THRESHOLD 30

// Ignore inputs (skip updates) [x]ms after state change
#define LOCKOUT_MS 30

// 0-1024. When does a signal become HIGH?
// Usually doesn't exceed 5, when not pressed
// ~900-950 in pressed transient, stabilizing around 1020
// Drops to 0-5 within a read cycle (Serial.print included, for what it's worth)
#define VOLTAGE_THRESHOLD 50

class ButtonHandler {
    
private:
    uint8_t _buttonPin;
    // ugly ass type syntax
    // pointer to a "(void) => void" lambda/function
    void (*_onPress)();
    void (*_onRelease)();
    
    bool lastPhysicalState;
    bool currentLogicalState;
    unsigned long _lastPhysicalStateChangeMs;
    unsigned long _lastLogicalStateChangeMs;
    
public:
    ButtonHandler(
        uint8_t buttonPin,
        void (*onPress)(),
        void (*onRelease)()
    );
    
    void init();
    
    // for the love of god,
    // please be invoked at least once every 20ms 🙏
    // or your warranty is void
    void update();
    
};
