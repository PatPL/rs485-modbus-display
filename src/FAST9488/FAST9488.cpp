#include "Print.h"
#include "HardwareSerial.h"
#include "SPI.h"
#include "Arduino.h"
#include "FAST9488.h"

inline bool FAST9488::startSPI() {
    // was already running,
    // send upkeep flag for later
    if (_spi) return true;
    _spi = true;
    SPI.beginTransaction(SPISettings(UINT32_MAX, MSBFIRST, SPI_MODE0));
    return false;
}

inline void FAST9488::stopSPI(bool upkeep) {
    if (upkeep) return;
    if (!_spi) return;
    _spi = false;
    SPI.endTransaction();
}

inline void FAST9488::write(uint8_t type, uint8_t data) {
    // TYPE_COMMAND or TYPE_DATA
    digitalWrite(_dc, type);
    digitalWrite(_cs, LOW);
    SPI.transfer(data);
    digitalWrite(_cs, HIGH);
}

inline void FAST9488::write(uint8_t type, uint8_t data, uint32_t length) {
    // TYPE_COMMAND or TYPE_DATA
    digitalWrite(_dc, type);
    digitalWrite(_cs, LOW);
    for (uint32_t i = 0; i < length; ++i) {
        SPI.transfer(data);
    }
    digitalWrite(_cs, HIGH);
}

inline void FAST9488::write(uint8_t type, Color data, uint32_t length) {
    // TYPE_COMMAND or TYPE_DATA
    digitalWrite(_dc, type);
    digitalWrite(_cs, LOW);
    for (uint32_t i = 0; i < length; ++i) {
        SPI.transfer(data.r);
        SPI.transfer(data.g);
        SPI.transfer(data.b);
    }
    digitalWrite(_cs, HIGH);
}

inline void FAST9488::setBufferBounds(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    write(TYPE_COMMAND, 0x2A); // Set columns (x); x0 to x1 inclusive
    write(TYPE_DATA, (x0 & 0xFF00) >> 8);
    write(TYPE_DATA, x0 & 0x00FF);
    write(TYPE_DATA, (x1 & 0xFF00) >> 8);
    write(TYPE_DATA, x1 & 0x00FF);
    
    write(TYPE_COMMAND, 0x2B); // Set rows (y); y0 to y1 inclusive
    write(TYPE_DATA, (y0 & 0xFF00) >> 8);
    write(TYPE_DATA, y0 & 0x00FF);
    write(TYPE_DATA, (y1 & 0xFF00) >> 8);
    write(TYPE_DATA, y1 & 0x00FF);
    
    write(TYPE_COMMAND, 0x2C);
}

inline void FAST9488::setRelativeBufferBounds(uint16_t x, uint16_t y, uint16_t xs, uint16_t ys) {
    setBufferBounds(x, y, x + xs - 1, y + ys - 1);
}

inline void FAST9488::setColorMode(bool reduced) {
    // Interface Pixel Format
    write(TYPE_COMMAND, 0x3A);
    // (through SPI) Supports:
    // * 0x11 -  3bits/px (00RGBRGB-00RGBRGB-00RGBRGB)
    // * 0x66 - 18bits/px (RRRRRR00-GGGGGG00-BBBBBB00)
    write(TYPE_DATA, reduced ? 0x11 : 0x66);
}

FAST9488::FAST9488(int8_t cs, int8_t reset, int8_t dc) {
    _cs = cs;
    _rst = reset;
    _dc = dc;
    
    _width = WIDTH_9488;
    _height = HEIGHT_9488;
    
    _spi = false;
}

void FAST9488::init() {
    pinMode(_cs, OUTPUT);
    pinMode(_rst, OUTPUT);
    pinMode(_dc, OUTPUT);
    SPI.begin();
    
    // HW RESET
    digitalWrite(_rst, LOW);
    delay(120);
    digitalWrite(_rst, HIGH);
    delay(20);
    
    // INITIAL CONFIGURATION
    bool keepSPI = startSPI();

    // Power Control 1
    write(TYPE_COMMAND, 0xC0);
    write(TYPE_DATA, 0x0F); //+4.5V
    write(TYPE_DATA, 0x0F); //-4.5V
    
    // Power Control 2
    write(TYPE_COMMAND, 0xC1);
    write(TYPE_DATA, 0x41); // Reduced "dither" artifacts in some colors
    
    // VCOM Control
    write(TYPE_COMMAND, 0xC5);
    write(TYPE_DATA, 0x00);
    write(TYPE_DATA, 0x11); // Reduced "dither" artifacts in some colors
    write(TYPE_DATA, 0x80);
    write(TYPE_DATA, 0x00);
    
    //Memory Access Control
    setOrientation(RIBBON_TOP);
    
    // Interface Pixel Format
    setColorMode(false); // not reduced (18 bit)
    
    // Interface Mode Control
    write(TYPE_COMMAND, 0xB0);
    write(TYPE_DATA, 0x80); // SDO (read) not used
    
    // Frame Rate Control
    write(TYPE_COMMAND, 0xB1);
    write(TYPE_DATA, 0x30); // ~20.038Hz
    write(TYPE_DATA, 0x1D); // DESMOS: \frac{18000000}{1\cdot\left(\left(31+1\right)\cdot2\right)\cdot29\cdot\left(480+2+2\right)}
    
    // Frame Rate Control (Idle)
    write(TYPE_COMMAND, 0xB2);
    write(TYPE_DATA, 0x00); // Same as Normal Mode (above)
    write(TYPE_DATA, 0x1D);
    
    // Frame Rate Control (Partial)
    write(TYPE_COMMAND, 0xB3);
    write(TYPE_DATA, 0x00); // Same as Normal Mode (above)
    write(TYPE_DATA, 0x1D);
    
    // Display Function Control
    write(TYPE_COMMAND, 0xB6);
    write(TYPE_DATA, 0x02);
    write(TYPE_DATA, 0x42); // Scan away from ribbon cable
    
    // Set Image Function
    write(TYPE_COMMAND, 0xE9);
    write(TYPE_DATA, 0x00); // No 24 bit data
    
    // Adjust Control 3
    write(TYPE_COMMAND, 0xF7);
    write(TYPE_DATA, 0xA9);
    write(TYPE_DATA, 0x51);
    write(TYPE_DATA, 0x2C);
    write(TYPE_DATA, 0x82);
    
    // Sleep OUT
    write(TYPE_COMMAND, 0x11);
    delay(5); // Required
    
    // Display ON
    write(TYPE_COMMAND, 0x29);
    
    // Normal Display Mode ON
    write(TYPE_COMMAND, 0x13);
    
    stopSPI(keepSPI);
}

void FAST9488::setOrientation(uint8_t o) {
    bool keepSPI = startSPI();
    
    //Memory Access Control
    write(TYPE_COMMAND, 0x36);
    switch(o) {
        case RIBBON_TOP:
            write(TYPE_DATA, 0x08);
            _width = WIDTH_9488;
            _height = HEIGHT_9488;
        break; case RIBBON_RIGHT:
            write(TYPE_DATA, 0xA8);
            _width = HEIGHT_9488;
            _height = WIDTH_9488;
        break; case RIBBON_BOTTOM:
            write(TYPE_DATA, 0xC8);
            _width = WIDTH_9488;
            _height = HEIGHT_9488;
        break; case RIBBON_LEFT:
            write(TYPE_DATA, 0x68);
            _width = HEIGHT_9488;
            _height = WIDTH_9488;
        break;
    }
    
    stopSPI(keepSPI);
}

void FAST9488::fill(Color color) {
    fillRect(0, 0, _width, _height, color);
}

void FAST9488::fill(bool r, bool g, bool b) {
    bool keepSPI = startSPI();
    
    setColorMode(true);
    setBufferBounds(0, 0, _width - 1, _height - 1);
    uint8_t color = 0;
    if (r) color |= 0b00100100;
    if (g) color |= 0b00010010;
    if (b) color |= 0b00001001;
    write(TYPE_DATA, color, ((uint32_t) _width) * _height / 2);
    setColorMode(false);
    
    // temp fix for black pixels dumped by changing color mode
    // setBufferBounds(UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX);
    
    stopSPI(keepSPI);
}

// <x0, x1) <y0, y1)
void FAST9488::fillRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Color color) {
    bool keepSPI = startSPI();
    
    setBufferBounds(x0, y0, x1 - 1, y1 - 1);
    write(TYPE_DATA, color, ((uint32_t) (x1 - x0)) * (y1 - y0));
    
    stopSPI(keepSPI);
}

// <x0, x1) <y0, y1)
void FAST9488::fillRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, bool r, bool g, bool b) {
    bool keepSPI = startSPI();
    
    setColorMode(true);
    setBufferBounds(x0, y0, x1 - 1, y1 - 1);
    uint8_t color = 0;
    if (r) color |= 0b00100100;
    if (g) color |= 0b00010010;
    if (b) color |= 0b00001001;
    write(TYPE_DATA, color, ((uint32_t) (x1 - x0)) * (y1 - y0) / 2);
    setColorMode(false);
    
    // temp fix for black pixels dumped by changing color mode
    // setBufferBounds(UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX);
    
    stopSPI(keepSPI);
}

// <x0, x1) <y>
inline void FAST9488::drawLineH(uint16_t x0, uint16_t x1, uint16_t y, Color color) {
    fillRect(x0, y, x1, y + 1, color);
}

// <x0, x1) <y>
inline void FAST9488::drawLineH(uint16_t x0, uint16_t x1, uint16_t y, bool r, bool g, bool b) {
    fillRect(x0, y, x1, y + 1, r, g, b);
}

// <x> <y0, y1)
inline void FAST9488::drawLineV(uint16_t y0, uint16_t y1, uint16_t x, Color color) {
    fillRect(x, y0, x + 1, y1, color);
}

// <x> <y0, y1)
inline void FAST9488::drawLineV(uint16_t y0, uint16_t y1, uint16_t x, bool r, bool g, bool b) {
    fillRect(x, y0, x + 1, y1, r, g, b);
}

// <x> <y>
inline void FAST9488::drawPixel(uint16_t x, uint16_t y, Color color) {
    fillRect(x, y, x + 1, y + 1, color);
}

// (x, y) - character bottomleft.
// bitmap, glyph - present in PROGMEM
inline void FAST9488::drawChar(uint16_t _x, uint16_t _y, uint8_t* bitmap, GFXglyph* glyph, Color color, uint8_t scale) {
    uint16_t bitmapOffset = pgm_read_word(&(glyph->bitmapOffset));
    uint8_t width = pgm_read_byte(&(glyph->width));
    uint8_t height = pgm_read_byte(&(glyph->height));
    int8_t xOffset = pgm_read_byte(&(glyph->xOffset));
    int8_t yOffset = pgm_read_byte(&(glyph->yOffset));
    
    uint8_t buf = 0;
    uint8_t bi = 0; // bit iterator
    for (uint8_t y = 0; y < height; ++y) {
        for (uint8_t x = 0; x < width; ++x) {
            if (bi == 0) {
                buf = pgm_read_byte(bitmap + bitmapOffset++);
                bi = 8;
            }
            
            if (buf & 0b10000000) {
                setRelativeBufferBounds(_x + ((x + xOffset) * scale), _y + ((y + yOffset) * scale), scale, scale);
                write(TYPE_DATA, color, scale * scale);
            }
            
            buf <<= 1;
            --bi;
        }
    }
}

uint16_t FAST9488::drawText(uint16_t _x, uint16_t _y, uint8_t anchor, const char* _s, Color color, const GFXfont* font, uint8_t scale) {
    bool keepSPI = startSPI();
    // 
    uint16_t x = _x;
    uint16_t y = anchor & ANCHOR_TOP ? (_y + (font->yAdvance * scale) - 1) : _y;
    y -= (font->yBasement + 1);
    unsigned char* ci; // character iterator
    uint8_t charStartOffset = font->first;
    GFXglyph* cig; // PGM
    
    if (anchor & ANCHOR_LEFT) {
        // anchor left
        ci = _s;
        while (*ci) {
            cig = font->glyph + *ci - charStartOffset;
            drawChar(x, y, font->bitmap, cig, color, scale);
            x += pgm_read_byte(&(cig->xAdvance)) * scale;
            ++ci;
        }
    } else {
        // anchor right
        // reverse the string
        uint8_t length = strlen(_s);
        unsigned char* _rs = new char[length + 1];
        for (uint8_t i = 0; i < length; ++i) {
            _rs[i] = _s[length - i - 1];
        }
        _rs[length] = '\0';
        ci = _rs;
        while (*ci) {
            cig = font->glyph + *ci - charStartOffset;
            x -= pgm_read_byte(&(cig->xAdvance)) * scale;
            drawChar(x, y, font->bitmap, cig, color, scale);
            ++ci;
        }
        
        x = _x + (_x - x);
        delete[] _rs;
    }
    
    stopSPI(keepSPI);
    return x - _x;
}

// Pallet clone version
void FAST9488::drawIcon(uint16_t _x, uint16_t _y, uint8_t anchor, PaletteMap* icon, uint8_t scale) {
    bool keepSPI = startSPI();
    // Icon data is laid out from top-left
    uint16_t x = _x - (anchor & ANCHOR_RIGHT ? icon->width * scale : 0);
    uint16_t y = _y - (anchor & ANCHOR_BOTTOM ? icon->height * scale : 0);
    
    Color* palette = new Color[icon->colorCount];
    for (int i = 0; i < icon->colorCount; ++i) {
        palette[i].r = pgm_read_byte((void*)(icon->palette + i) + 0);
        palette[i].g = pgm_read_byte((void*)(icon->palette + i) + 1);
        palette[i].b = pgm_read_byte((void*)(icon->palette + i) + 2);
    }
    
    uint8_t bpc = icon->bitsPerColor;
    uint8_t color = 0;
    uint32_t buf = 0;
    uint32_t bufMask = 0xFFFFFFFF >> (32 - bpc);
    uint8_t bufI = 0;
    uint16_t mapI = 0;
    for (int iY = 0; iY < icon->height; ++iY) {
        for (int iX = 0; iX < icon->width; ++iX) {
            if (bufI < bpc) {
                buf |= (uint32_t)pgm_read_word(icon->map + mapI) << (bufI);
                bufI += 16;
                ++mapI;
            }
            
            color = buf & bufMask;
            if (color != 0) {
                setRelativeBufferBounds(x + iX * scale, y + iY * scale, scale, scale);
                write(TYPE_DATA, palette[color - 1], scale * scale);
            }
            
            buf >>= bpc;
            bufI -= bpc;
        }
    }
    
    delete[] palette;
    stopSPI(keepSPI);
}
