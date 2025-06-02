#pragma once

#include <stdint.h>
#include "../FAST9488.h"

class TextUpdater {
    
private:
    FAST9488* _ftft;
    uint16_t _x;
    uint16_t _y;
    uint8_t _anchor;
    Color _bg;
    bool _bulkClear;
    GFXfont* _font;
    uint8_t _scale;
    
    union {
        // _bulkClear = false
        char* currentTextContent;
        
        // when _bulkClear = true
        uint16_t currentTextPixelLength;
    } _clearData;
    
public:
    TextUpdater(
        FAST9488* ftft,
        uint8_t bufLen,
        uint16_t x,
        uint16_t y,
        uint8_t anchor,
        Color _fg,
        Color bg,
        bool bulkClear,
        GFXfont* font,
        uint8_t scale = 1
    );
    
    Color fg;
    char* buf;
    void update();
    
};
