#include "ArrowManager.h"

ArrowManager::ArrowManager(
    FAST9488* ftft,
    uint16_t x0,
    uint16_t y0,
    uint16_t x1,
    uint16_t y1,
    bool horizontalHigh,
    uint8_t width,
    Color _fg,
    Color bg,
    bool _reverseDirection
) {
    _ftft = ftft;
    _x0 = x0;
    _y0 = y0;
    _x1 = x1;
    _y1 = y1;
    _horizontalHigh = horizontalHigh;
    _width = width;
    fg = _fg;
    _bg = bg;
    reverseDirection = _reverseDirection;
    _drawnDirection = _reverseDirection;
}

void ArrowManager::update(bool skipMove) {
    if (_unflushedMove && !skipMove) {
        clear();
    }
    
    uint8_t w1 = (_width - 1) / 2;
    uint8_t w2 = _width / 2;
    bool horizontalHead = (
        (_x0 != _x1) && (
            (_y0 == _y1) ||
            (_horizontalHigh && _y1 < _y0) ||
            (!_horizontalHigh && _y1 > _y0)
        )
    );
    
    // todo: check if rewrite with pointers uses less flash space
    //       both cases are pretty much the same, just with swapped x/y
    //       (check all TODO occurences in method)
    // clean old arrowhead's fins, if direction changed
    if (_drawnDirection != reverseDirection) {
        if (horizontalHead) {
            bool dirLeft = _x1 < _x0;
            int8_t finDir = dirLeft ? 1 : -1;
            
            uint16_t finX0 = (dirLeft ? _x1              : _x1 - _width) + w1 * finDir;
            uint16_t finX1 = (dirLeft ? _x1 + _width + 1 : _x1 + 1) + w1 * finDir;
            
            for (uint8_t i = w1; i < _width + 1 + w1; ++i) {
                finX0 += finDir;
                finX1 += finDir;
                // top
                _ftft->fillRect(finX0, _y1 - 1 - i, finX1, _y1 - i, _bg);
                // bottom
                _ftft->fillRect(finX0, _y1 + w2 - w1 + 1 + i, finX1, _y1 + w2 - w1 + 2 + i, _bg);
            }
            
        } else {
            bool dirTop = _y1 < _y0;
            int8_t finDir = dirTop ? 1 : -1;
            
            uint16_t finY0 = (dirTop ? _y1              : _y1 - _width) + w1 * finDir;
            uint16_t finY1 = (dirTop ? _y1 + _width + 1 : _y1 + 1) + w1 * finDir;
            
            for (uint8_t i = w1; i < _width + 1 + w1; ++i) {
                finY0 += finDir;
                finY1 += finDir;
                // left
                _ftft->fillRect(_x1 - 1 - i, finY0, _x1 - i, finY1, _bg);
                // right
                _ftft->fillRect(_x1 + w2 - w1 + 1 + i, finY0, _x1 + w2 - w1 + 2 + i, finY1, _bg);
            }
        }
        
        // prepare next arrowhead's tip, if direction changed
        bool horizontalEnd = (
            ((_x0 == _x1 || _y0 == _y1) && horizontalHead) ||
            ((_x0 != _x1 && _y0 != _y1) && !horizontalHead)
        );
        
        if (horizontalEnd) {
            int8_t trimDir = _x1 < _x0 ? -1 : 1;
            for (int i = 0; i < w1; ++i) {
                _ftft->fillRect(_x0 + (i * trimDir), _y0 - w1, _x0 + (i * trimDir) + 1, _y0 + i, _bg);
                _ftft->fillRect(_x0 + (i * trimDir), _y0 + 1 + w2 - w1, _x0 + (i * trimDir) + 1, _y0 + 1 + w2, _bg);
            }
        } else {
            int8_t trimDir = _y1 < _y0 ? -1 : 1;
            for (int i = 0; i < w1; ++i) {
                _ftft->fillRect(_x0 - w1, _y0 + (i * trimDir), _x0 - i, _y0 + (i * trimDir) + 1, _bg);
                _ftft->fillRect(_x0 + i + 1 + w2 - w1, _y0 + (i * trimDir), _x0 + 1 + w2, _y0 + (i * trimDir) + 1, _bg);
            }
        }
        
        // swap coords, draw rest as usual.
        _x0 = _x0 + _x1;
        _x1 = _x0 - _x1;
        _x0 = _x0 - _x1;
        _y0 = _y0 + _y1;
        _y1 = _y0 - _y1;
        _y0 = _y0 - _y1;
        _drawnDirection = reverseDirection;
        // recalculation required
        horizontalHead = (
            (_x0 != _x1) && (
                (_y0 == _y1) ||
                (_horizontalHigh && _y1 < _y0) ||
                (!_horizontalHigh && _y1 > _y0)
            )
        );
    }
    
    // todo: check if rewrite with pointers uses less flash space
    //       both cases are pretty much the same, just with swapped x/y
    //       (check all TODO occurences in method)
    // always draw last part of the arrow (shaft)
    if (horizontalHead) {
        _ftft->fillRect(
            _x1 < _x0 ? _x1 + _width + 1 : _x0,
            _y1 - w1,
            _x1 < _x0 ? _x0 + 1 : _x1 - _width,
            _y1 + w2 + 1,
            fg
        );
    } else {
        _ftft->fillRect(
            _x1 - w1,
            _y1 < _y0 ? _y1 + _width + 1 : _y0,
            _x1 + 1 + w2,
            _y1 < _y0 ? _y0 + 1 : _y1 - _width,
            fg
        );
    }
    
    // todo: check if rewrite with pointers uses less flash space
    //       both cases are pretty much the same, just with swapped x/y
    //       (check all TODO occurences in method)
    // draw tip & fins
    if (horizontalHead) {
        // ↔
        bool dirLeft = _x1 < _x0;
        uint16_t finX0 = dirLeft ? _x1              : _x1 - _width;
        uint16_t finX1 = dirLeft ? _x1 + _width + 1 : _x1 + 1;
        int8_t finDir = dirLeft ? 1 : -1;
        
        // center
        _ftft->fillRect(finX0, _y1, finX1, _y1 + 1 + w2 - w1, fg);
        
        for (uint8_t i = 0; i < _width + 1 + w1; ++i) {
            finX0 += finDir;
            finX1 += finDir;
            // top
            _ftft->fillRect(finX0, _y1 - 1 - i, finX1, _y1 - i, fg);
            // bottom
            _ftft->fillRect(finX0, _y1 + w2 - w1 + 1 + i, finX1, _y1 + w2 - w1 + 2 + i, fg);
        }
        
    } else {
        // ↕
        bool dirTop = _y1 < _y0;
        uint16_t finY0 = dirTop ? _y1              : _y1 - _width;
        uint16_t finY1 = dirTop ? _y1 + _width + 1 : _y1 + 1;
        int8_t finDir = dirTop ? 1 : -1;
        
        // center
        _ftft->fillRect(_x1, finY0, _x1 + 1 + w2 - w1, finY1, fg);
        
        for (uint8_t i = 0; i < _width + 1 + w1; ++i) {
            finY0 += finDir;
            finY1 += finDir;
            // left
            _ftft->fillRect(_x1 - 1 - i, finY0, _x1 - i, finY1, fg);
            // right
            _ftft->fillRect(_x1 + w2 - w1 + 1 + i, finY0, _x1 + w2 - w1 + 2 + i, finY1, fg);
        }
    }
    
    // todo: check if rewrite with pointers uses less flash space
    //       both cases are pretty much the same, just with swapped x/y
    //       (check all TODO occurences in method)
    // draw first part up to corner, if exists
    if (_x0 != _x1 && _y0 != _y1) {
        if (horizontalHead) {
            // ↰ ↱ ↲ ↳ (vertical shaft)
            _ftft->fillRect(
                _x0 - w1,
                _y1 < _y0 ? _y1 - w1 : _y0,
                _x0 + w2 + 1,
                _y1 < _y0 ? _y0 + 1 : _y1 + w2 + 1,
                fg
            );
        } else {
            // ↴ (horizontal shaft)
            _ftft->fillRect(
                _x1 < _x0 ? _x1 - w1 : _x0,
                _y0 - w1,
                _x1 < _x0 ? _x0 + 1 : _x1 + w2 + 1,
                _y0 + w2 + 1,
                fg
            );
        }
    }
    
}

void ArrowManager::clear() {
    bool revSave = reverseDirection;
    Color fgSave = fg;
    
    fg = _bg;
    reverseDirection = _drawnDirection;
    
    update(true);
    
    if (_unflushedMove) {
        _x0 = _unflushedMove->newX0;
        _y0 = _unflushedMove->newY0;
        _x1 = _unflushedMove->newX1;
        _y1 = _unflushedMove->newY1;
        _width = _unflushedMove->newWidth;
        
        delete _unflushedMove;
        _unflushedMove = nullptr;
    }
    
    fg = fgSave;
    _drawnDirection = revSave;
    reverseDirection = revSave;
}

void ArrowManager::move(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t width) {
    if (_unflushedMove) delete _unflushedMove;
    _unflushedMove = new NewMove { x0, y0, x1, y1, width ? width : _width };
}