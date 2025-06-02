#include "TextUpdater.h"

TextUpdater::TextUpdater(
    FAST9488* ftft,
    uint8_t bufLen,
    uint16_t x,
    uint16_t y,
    uint8_t anchor,
    Color _fg,
    Color bg,
    bool bulkClear,
    const GFXfont* font,
    uint8_t scale
) {
    _ftft = ftft;
    _x = x;
    _y = y;
    _anchor = anchor;
    fg = _fg;
    _bg = bg;
    _bulkClear = bulkClear;
    _font = (GFXfont*)font;
    _scale = scale;
    
    buf = new char[bufLen];
    buf[0] = '\0';
    if (_bulkClear) {
        _clearData.currentTextPixelLength = 0;
    } else {
        _clearData.currentTextContent = new char[bufLen];
        _clearData.currentTextContent[0] = '\0';
    }
    
}

void TextUpdater::update() {
    if (_bulkClear) {
        switch (_anchor) {
            /****/ case ANCHOR_BOTTOMLEFT:
                _ftft->fillRect(_x, _y - (_font->yAdvance - _font->yBasement) * _scale, _x + _clearData.currentTextPixelLength, _y + _font->yBasement * _scale, _bg);
            break; case ANCHOR_BOTTOMRIGHT:
                _ftft->fillRect(_x - _clearData.currentTextPixelLength, _y - (_font->yAdvance - _font->yBasement) * _scale, _x, _y + _font->yBasement * _scale, _bg);
            break; case ANCHOR_TOPLEFT:
                _ftft->fillRect(_x, _y + _font->yBasement * _scale, _x + _clearData.currentTextPixelLength, _y + (_font->yAdvance+ _font->yBasement) * _scale, _bg);
            break; case ANCHOR_TOPRIGHT:
                _ftft->fillRect(_x - _clearData.currentTextPixelLength, _y + _font->yBasement * _scale, _x, _y + (_font->yAdvance+ _font->yBasement) * _scale, _bg);
            break;
        }
        
        _clearData.currentTextPixelLength = _ftft->drawText(_x, _y, _anchor, buf, fg, _font, _scale);
    } else {
        _ftft->drawText(_x, _y, _anchor, _clearData.currentTextContent, _bg, _font, _scale);
        strcpy(_clearData.currentTextContent, buf);
        
        _ftft->drawText(_x, _y, _anchor, buf, fg, _font, _scale);
    }
}