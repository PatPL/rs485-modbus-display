#pragma once
#include<stdint.h>

// This will be used sparingly during initialization, and button events.
// Performance was not a concern. RAM usage was.

template <typename T>
struct Vector8 {
    
private:
    uint8_t _size = 0;
    T* _data = nullptr;

public:
    T operator[](uint8_t index);
    void push(T item);
    void pop();
    void remove(uint8_t index);
    void clear();
    
    #ifdef __DEBUG
    #include "Arduino.h"
    void debug();
    #endif
};

// Template definitions must be in the header file
// What's even the point of splitting into .h/.cpp
#include "Vector8.cpp"
