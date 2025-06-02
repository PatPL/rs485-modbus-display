#pragma once

#include <stdint.h>
#include "Arduino.h"

#define TYPE_COMMAND LOW
#define TYPE_DATA HIGH

#define WIDTH_9488 320
#define HEIGHT_9488 480

#define RIBBON_TOP 0
#define RIBBON_RIGHT 1
#define RIBBON_BOTTOM 2
#define RIBBON_LEFT 3

#define ANCHOR_TOP      0b0001
#define ANCHOR_BOTTOM   0b0010
#define ANCHOR_LEFT     0b0100
#define ANCHOR_RIGHT    0b1000
#define ANCHOR_TOPRIGHT ANCHOR_TOP | ANCHOR_RIGHT
#define ANCHOR_TOPLEFT ANCHOR_TOP | ANCHOR_LEFT
#define ANCHOR_BOTTOMRIGHT ANCHOR_BOTTOM | ANCHOR_RIGHT
#define ANCHOR_BOTTOMLEFT ANCHOR_BOTTOM | ANCHOR_LEFT

// <Stolen from Adafruit libraries>
/// Font data stored PER GLYPH
typedef struct {
    uint16_t bitmapOffset; ///< Pointer into GFXfont->bitmap
    uint8_t width;         ///< Bitmap dimensions in pixels
    uint8_t height;        ///< Bitmap dimensions in pixels
    uint8_t xAdvance;      ///< Distance to advance cursor (x axis)
    int8_t xOffset;        ///< X dist from cursor pos to UL corner
    int8_t yOffset;        ///< Y dist from cursor pos to UL corner
} GFXglyph;

/// Data stored for FONT AS A WHOLE
typedef struct {
    uint8_t *bitmap;  ///< Glyph bitmaps, concatenated
    GFXglyph *glyph;  ///< Glyph array
    uint16_t first;   ///< ASCII extents (first char)
    uint16_t last;    ///< ASCII extents (last char)
    uint8_t yAdvance; ///< Newline distance (y axis)
    uint8_t yBasement;///< How far under the cursor baseline do the fonts extend
} GFXfont;
// </Stolen from Adafruit libraries>

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct PaletteMap {
    uint16_t* map;           // 0: transparent (no draw)
    Color* palette;         // max size: (2^bitsPerColor)-1
    uint8_t colorCount;
    uint8_t bitsPerColor;
    uint8_t width;
    uint8_t height;
};

class FAST9488 {

private:
    // SPI pins
    int8_t _cs;
    int8_t _rst;
    int8_t _dc;
    
    int16_t _width;
    int16_t _height;
    
    bool _spi;
    
    inline bool startSPI();
    inline void stopSPI(bool upkeep);
    
    inline void write(uint8_t type, uint8_t data);
    inline void write(uint8_t type, uint8_t data, uint32_t length);
    inline void write(uint8_t type, Color data, uint32_t length);
    inline void setBufferBounds(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    inline void setRelativeBufferBounds(uint16_t x, uint16_t y, uint16_t xs, uint16_t ys);
    inline void setColorMode(bool reduced);
    inline void drawChar(uint16_t _x, uint16_t _y, uint8_t* bitmap, GFXglyph* glyph, Color color, uint8_t scale);
public:
    FAST9488(int8_t cs, int8_t reset, int8_t dc);
    
    void init();
    
    void setOrientation(uint8_t o);
    void flush(bool force = false, bool full = false);
    
    void fill(Color color);
    void fill(bool r, bool g, bool b);
    void fillRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Color color);
    void fillRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, bool r, bool g, bool b);
    
    inline void drawLineH(uint16_t x0, uint16_t x1, uint16_t y, Color color);
    inline void drawLineH(uint16_t x0, uint16_t x1, uint16_t y, bool r, bool g, bool b);
    inline void drawLineV(uint16_t y0, uint16_t y1, uint16_t x, Color color);
    inline void drawLineV(uint16_t y0, uint16_t y1, uint16_t x, bool r, bool g, bool b);
    inline void drawPixel(uint16_t x, uint16_t y, Color color);
    
    uint16_t drawText(uint16_t _x, uint16_t _y, uint8_t anchor, const char* s, Color color, const GFXfont* font, uint8_t scale = 1);
    void drawIcon(uint16_t _x, uint16_t _y, uint8_t anchor, const PaletteMap* icon, uint8_t scale = 1);
};
