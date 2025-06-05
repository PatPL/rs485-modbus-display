#pragma once

#include "../FAST9488.h"



class ArrowManager {
    
private:
    
    struct NewMove {
        uint16_t newX0;
        uint16_t newY0;
        uint16_t newX1;
        uint16_t newY1;
        uint8_t newWidth;
    };
public: // tmp
    FAST9488* _ftft;
    uint16_t _x0;
    uint16_t _y0;
    uint16_t _x1;
    uint16_t _y1;
    bool _horizontalHigh; // true: ↰ ↱ ↴, false: ↵ ↳ ↲
    uint8_t _width;
    Color _bg;
    bool _drawnDirection;
    NewMove* _unflushedMove = nullptr;
//public:
    Color fg;
    // swaps 0<->1 on draw
    // arrow always goes 0->1
    bool reverseDirection;
    
    ArrowManager(
        FAST9488* ftft,
        uint16_t x0,
        uint16_t y0,
        uint16_t x1,
        uint16_t y1,
        bool horizontalHigh,
        uint8_t width,
        Color _fg,
        Color bg,
        bool _reverseDirection = false
    );
    void update(bool skipMove = false);
    void clear();
    void move(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t width = NULL);
};
