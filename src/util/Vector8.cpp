#pragma once
#include "Vector8.h"

template<typename T>
T Vector8<T>::operator[](uint8_t index) {
    return _data[index];
}

template<typename T>
void Vector8<T>::push(T item) {
    T* oldData = _data;
    ++_size;
    _data = new T[_size];
    
    memcpy(_data, oldData, sizeof(T) * (_size - 1));
    _data[_size - 1] = item;
    
    delete[] oldData;
}

template<typename T>
void Vector8<T>::pop() {
    T* oldData = _data;
    --_size;
    _data = new T[_size];
    
    memcpy(_data, oldData, sizeof(T) * _size);
    
    delete[] oldData;
}

template<typename T>
void Vector8<T>::remove(uint8_t index) {
    // if (index > _size) return;
    T* oldData = _data;
    --_size;
    _data = new T[_size];
    
    memcpy(_data, oldData, sizeof(T) * index);
    memcpy(_data + index, oldData + index + 1, sizeof(T) * (_size - index));
    
    delete[] oldData;
}

template<typename T>
void Vector8<T>::clear() {
    delete[] _data;
    _size = 0;
    _data = nullptr;
}

#ifdef __DEBUG
template<typename T>
void Vector8<T>::debug() {
    Serial.print("V8: (");
    for (uint16_t i = 0; i < _size * sizeof(T); ++i) {
        Serial.print("0x");
        Serial.print(((uint8_t*)_data)[i], HEX);
        if (i != _size * sizeof(T) - 1) Serial.print(", ");
    }
    Serial.println(")");
}
#endif
