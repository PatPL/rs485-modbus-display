#include "SPI.h"
#include "src/FAST9488/FAST9488.h"
#include "src/FAST9488/fonts/FreeSerif18pt7b.h"
#include "src/FAST9488/fonts/FreeSans12pt7b.h"
#include "src/FAST9488/util/TextUpdater.h"
#include "src/util/ButtonHandler.h"
#include "src/FAST9488/util/ArrowManager.h"
#include "icons/output/icons.h"

#define LED_R 3
#define LED_Y 4
#define OUT_1 6
#define OUT_2 7
#define OUT_3 8
#define BUTTON_L A1
#define BUTTON_M A2
#define BUTTON_R A3

// temporary
uint16_t c = 0;
TextUpdater* tu_counter = nullptr;

FAST9488 ftft = FAST9488(
    9,  // CS
    10, // RESET
    5  // DC/RS
);

void setup() {
    // RS-485
    Serial.begin(115200, SERIAL_8N1);
    
    // TFT
    ftft.init();
    ftft.setOrientation(RIBBON_BOTTOM);
    ftft.fill(false, false, false);
    
    pinMode(LED_R, OUTPUT);
    pinMode(LED_Y, OUTPUT);
    pinMode(OUT_1, OUTPUT);
    pinMode(OUT_2, OUTPUT);
    pinMode(OUT_3, OUTPUT);
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_Y, LOW);
    digitalWrite(OUT_1, LOW);
    digitalWrite(OUT_2, LOW);
    digitalWrite(OUT_3, LOW);
    
    pinMode(BUTTON_L, INPUT);
    pinMode(BUTTON_M, INPUT);
    pinMode(BUTTON_R, INPUT);
    
    arrow_test();
    
    // board_test();
}

void loop(void) {
    delay(1000);
}

ArrowManager ar1 = ArrowManager(
    &ftft,
    119,
    140,
    50,
    80,
    false,
    1,
    { 255, 32, 0 },
    { 255, 255, 255 }
);

ArrowManager ar2 = ArrowManager(
    &ftft,
    140,
    119,
    80,
    50,
    true,
    2,
    { 255, 0, 64 },
    { 255, 255, 255 }
);

ArrowManager ar3 = ArrowManager(
    &ftft,
    180,
    119,
    239,
    50,
    true,
    3,
    { 128, 255, 64 },
    { 255, 255, 255 }
);

ArrowManager ar4 = ArrowManager(
    &ftft,
    200,
    140,
    270,
    80,
    false,
    4,
    { 0, 255, 192 },
    { 255, 255, 255 }
);

ArrowManager ar5 = ArrowManager(
    &ftft,
    200,
    180,
    270,
    239,
    true,
    5,
    { 32, 192, 255 },
    { 255, 255, 255 }
);

ArrowManager ar6 = ArrowManager(
    &ftft,
    180,
    200,
    239,
    270,
    false,
    6,
    { 128, 128, 255 },
    { 255, 255, 255 }
);

ArrowManager ar7 = ArrowManager(
    &ftft,
    140,
    200,
    80,
    270,
    false,
    7,
    { 255, 224, 128 },
    { 255, 255, 255 }
);

ArrowManager ar8 = ArrowManager(
    &ftft,
    119,
    180,
    50,
    239,
    true,
    8,
    { 128, 32, 192 },
    { 255, 255, 255 }
);

ArrowManager ar11 = ArrowManager(
    &ftft,
    80,
    30,
    239,
    30,
    false,
    8,
    { 255, 0, 0 },
    { 255, 255, 255 }
);

ArrowManager ar12 = ArrowManager(
    &ftft,
    290,
    80,
    290,
    239,
    false,
    8,
    { 32, 255, 32 },
    { 255, 255, 255 }
);

ArrowManager ar13 = ArrowManager(
    &ftft,
    239,
    290,
    80,
    290,
    false,
    8,
    { 32, 64, 255 },
    { 255, 255, 255 }
);

ArrowManager ar14 = ArrowManager(
    &ftft,
    30,
    239,
    30,
    80,
    false,
    8,
    { 128, 128, 128 },
    { 255, 255, 255 }
);

ArrowManager ar21 = ArrowManager(
    &ftft,
    20,
    350,
    300,
    350,
    false,
    7,
    { 0, 0, 0 },
    { 255, 255, 255 }
);

ArrowManager ar22 = ArrowManager(
    &ftft,
    300,
    390,
    20,
    390,
    false,
    13,
    { 255, 128, 0 },
    { 255, 255, 255 }
);

void arrow_test() {
    ftft.fill(true, true, true);
    
    ftft.fillRect(20, 20, 20+60, 20+60, { 32, 32, 32 });
    ftft.fillRect(320-20-60, 20, 320-20, 20+60, { 64, 32, 32 });
    ftft.fillRect(20, 320-20-60, 20+60, 320-20, { 32, 64, 32 });
    ftft.fillRect(320-20-60, 320-20-60, 320-20, 320-20, { 32, 32, 64 });
    
    ftft.fillRect(120, 120, 200, 200, { 64, 64, 64 });
    
    ar1.update();
    ar2.update();
    ar3.update();
    ar4.update();
    ar5.update();
    ar6.update();
    ar7.update();
    ar8.update();
    Serial.println("START");
    ar11.update();
    Serial.println("END");
    ar12.update();
    ar13.update();
    ar14.update();
    ar21.update();
    ar22.update();
    
    tu_counter = new TextUpdater(&ftft, 64, 4, 330, ANCHOR_BOTTOMLEFT, { 0, 0, 0 }, { 255, 255, 255 }, true, &FreeSans12pt7b, 1);
    snprintf(tu_counter->buf, 64, "%d,%d>%d,%d|%d,%d>%d,%d", ar21._x0, ar21._y0, ar21._x1, ar21._y1, ar22._x0, ar22._y0, ar22._x1, ar22._y1);
    // tu_counter->update();
    
    ButtonHandler leftButton = ButtonHandler(
        BUTTON_L,
        []{
            ar21.move(random(15+40, 305-40), random(0, 110) + 320, random(15+40, 305-40), random(0, 110) + 320);
            ar22.move(random(15+40, 305-40), random(0, 110) + 320, random(15+40, 305-40), random(0, 110) + 320, random(1,33));
            ar22.fg = { (uint8_t) random(0, 255), (uint8_t) random(0, 255), (uint8_t) random(0, 255) };
            
            ar21.clear();
            ar22.clear();
            
            snprintf(tu_counter->buf, 64, "%d,%d>%d,%d|%d,%d>%d,%d", ar21._x0, ar21._y0, ar21._x1, ar21._y1, ar22._x0, ar22._y0, ar22._x1, ar22._y1);
            // tu_counter->update();
            
            ar21.update();
            ar22.update();
        }, nullptr
    );
    ButtonHandler rightButton = ButtonHandler(
        BUTTON_R,
        []{
            ar1.reverseDirection = !ar1.reverseDirection;
            ar2.reverseDirection = !ar2.reverseDirection;
            ar3.reverseDirection = !ar3.reverseDirection;
            ar4.reverseDirection = !ar4.reverseDirection;
            ar5.reverseDirection = !ar5.reverseDirection;
            ar6.reverseDirection = !ar6.reverseDirection;
            ar7.reverseDirection = !ar7.reverseDirection;
            ar8.reverseDirection = !ar8.reverseDirection;
            ar11.reverseDirection = !ar11.reverseDirection;
            ar12.reverseDirection = !ar12.reverseDirection;
            ar13.reverseDirection = !ar13.reverseDirection;
            ar14.reverseDirection = !ar14.reverseDirection;
            ar21.reverseDirection = !ar21.reverseDirection;
            ar22.reverseDirection = !ar22.reverseDirection;
            
            ar1.update();
            ar2.update();
            ar3.update();
            ar4.update();
            ar5.update();
            ar6.update();
            ar7.update();
            ar8.update();
            ar11.update();
            ar12.update();
            ar13.update();
            ar14.update();
            ar21.update();
            ar22.update();
        }, nullptr
    );
    ButtonHandler middleButton = ButtonHandler(
        BUTTON_M,
        []{
            ar1.fg = { (uint8_t)random(0, 256), (uint8_t)random(0, 256), (uint8_t)random(0, 256) };
            ar2.fg = { (uint8_t)random(0, 256), (uint8_t)random(0, 256), (uint8_t)random(0, 256) };
            ar3.fg = { (uint8_t)random(0, 256), (uint8_t)random(0, 256), (uint8_t)random(0, 256) };
            ar4.fg = { (uint8_t)random(0, 256), (uint8_t)random(0, 256), (uint8_t)random(0, 256) };
            ar5.fg = { (uint8_t)random(0, 256), (uint8_t)random(0, 256), (uint8_t)random(0, 256) };
            ar6.fg = { (uint8_t)random(0, 256), (uint8_t)random(0, 256), (uint8_t)random(0, 256) };
            ar7.fg = { (uint8_t)random(0, 256), (uint8_t)random(0, 256), (uint8_t)random(0, 256) };
            ar8.fg = { (uint8_t)random(0, 256), (uint8_t)random(0, 256), (uint8_t)random(0, 256) };
            ar11.fg = { (uint8_t)random(0, 256), (uint8_t)random(0, 256), (uint8_t)random(0, 256) };
            ar12.fg = { (uint8_t)random(0, 256), (uint8_t)random(0, 256), (uint8_t)random(0, 256) };
            ar13.fg = { (uint8_t)random(0, 256), (uint8_t)random(0, 256), (uint8_t)random(0, 256) };
            ar14.fg = { (uint8_t)random(0, 256), (uint8_t)random(0, 256), (uint8_t)random(0, 256) };
            ar1.reverseDirection = (bool) random(0, 2);
            ar2.reverseDirection = (bool) random(0, 2);
            ar3.reverseDirection = (bool) random(0, 2);
            ar4.reverseDirection = (bool) random(0, 2);
            ar5.reverseDirection = (bool) random(0, 2);
            ar6.reverseDirection = (bool) random(0, 2);
            ar7.reverseDirection = (bool) random(0, 2);
            ar8.reverseDirection = (bool) random(0, 2);
            ar11.reverseDirection = (bool) random(0, 2);
            ar12.reverseDirection = (bool) random(0, 2);
            ar13.reverseDirection = (bool) random(0, 2);
            ar14.reverseDirection = (bool) random(0, 2);
            
            ar21.move(random(15+40, 305-40), random(0, 110) + 320, random(15+40, 305-40), random(0, 110) + 320);
            ar22.move(random(15+40, 305-40), random(0, 110) + 320, random(15+40, 305-40), random(0, 110) + 320, random(1,33));
            ar22.fg = { (uint8_t) random(0, 255), (uint8_t) random(0, 255), (uint8_t) random(0, 255) };
            
            ar21.clear();
            ar22.clear();
            
            snprintf(tu_counter->buf, 64, "%d,%d>%d,%d|%d,%d>%d,%d", ar21._x0, ar21._y0, ar21._x1, ar21._y1, ar22._x0, ar22._y0, ar22._x1, ar22._y1);
            // tu_counter->update();
            
            ar1.update();
            ar2.update();
            ar3.update();
            ar4.update();
            ar5.update();
            ar6.update();
            ar7.update();
            ar8.update();
            ar11.update();
            ar12.update();
            ar13.update();
            ar14.update();
            ar21.update();
            ar22.update();
        }, nullptr
    );
    
    leftButton.init();
    rightButton.init();
    middleButton.init();
    
    while (true) {
        leftButton.update();
        rightButton.update();
        middleButton.update();
        delay(1);
    }
}

void board_test() {
    
    switch (2) {
        case 1: {
            ftft.fillRect(40, 40, 40 + 240, 40 + 60, true, true, false);
            ftft.drawText(40, 40 + 60, ANCHOR_BOTTOMLEFT, "@WIWIyqp|[]()", { 255, 0, 0 }, &FreeSans12pt7b, 1);
            ftft.drawText(40 + 240, 40, ANCHOR_TOPRIGHT, "@WIWIyqp|[]()", { 0, 64, 0 }, &FreeSerif18pt7b, 1);
            ftft.fillRect(40, 40 + 80, 40 + 240, 40 + 80 + 60, true, true, true);
            ftft.drawText(40, 40 + 80, ANCHOR_TOPLEFT, "@WIWIyqp|[]()", { 0, 0, 255 }, &FreeSans12pt7b, 1);
            ftft.drawText(40 + 240, 40 + 80 + 60, ANCHOR_BOTTOMRIGHT, "@WIWIyqp|[]()", { 0, 0, 0 }, &FreeSerif18pt7b, 1);
            ftft.fillRect(40, 40 + 160, 40 + 240, 40 + 160 + 60, false, true, true);
            ftft.drawText(40 + 240, 40 + 160 + 60, ANCHOR_BOTTOMRIGHT, "@WIWIyqp|[]()", { 255, 0, 255 }, &FreeSans12pt7b, 1);
            ftft.drawText(40, 40 + 160, ANCHOR_TOPLEFT, "@WIWIyqp|[]()", { 64, 64, 64 }, &FreeSerif18pt7b, 1);
            ftft.fillRect(40, 40 + 240, 40 + 240, 40 + 240 + 60, true, true, true);
            ftft.drawText(40 + 240, 40 + 240, ANCHOR_TOPRIGHT, "@WIWIyqp|[]()", { 0, 96, 64 }, &FreeSans12pt7b, 1);
            ftft.drawText(40, 40 + 240 + 60, ANCHOR_BOTTOMLEFT, "@WIWIyqp|[]()", { 0, 96, 128 }, &FreeSerif18pt7b, 1);
        } break; case 2: {
            ftft.fillRect(16, 16, 16+80, 16+80, { 32, 32, 32 });
            ftft.drawIcon(16, 16, ANCHOR_TOPLEFT, &icon_icontest);
            ftft.drawIcon(16+80, 16, ANCHOR_TOPRIGHT, &icon_icontest);
            ftft.drawIcon(16, 16+80, ANCHOR_BOTTOMLEFT, &icon_icontest);
            ftft.drawIcon(16+80-32, 16+80-32, ANCHOR_TOPLEFT, &icon_icontest, 2);
            
            ftft.drawIcon(16+80-32+64+4, 16, ANCHOR_TOPLEFT, &icon_monotest, 1);
            ftft.drawIcon(16+80-32+64+4+17+4, 16, ANCHOR_TOPLEFT, &icon_monotest, 2);
            ftft.drawIcon(16+80-32+64+4+17+4+34+4, 16, ANCHOR_TOPLEFT, &icon_monotest, 3);
            ftft.drawIcon(16+80-32+64+4+17+4+34+4+51+4, 16, ANCHOR_TOPLEFT, &icon_monotest, 4);
            
            ftft.drawIcon(16+80-32+64+4+17+4+34+4+51+4+68, 16+41*4+4, ANCHOR_TOPRIGHT, &icon_3bittest, 3);
            
            ftft.drawIcon(16, 16+80-32+64+8, ANCHOR_TOPLEFT, &icon_6bittest, 2);
        }
    }
    
    uint16_t t_o = ftft.drawText(16+80, 480-16-64, ANCHOR_BOTTOMLEFT, "counter: ", { 255, 255, 255 }, &FreeSerif18pt7b, 1);
    
    tu_counter = new TextUpdater(&ftft, 8, 16+80+t_o, 480-16-64, ANCHOR_BOTTOMLEFT, { 96, 96, 96 }, { 0, 0, 0 }, true, &FreeSans12pt7b, 2);
    itoa(c, tu_counter->buf, 10);
    tu_counter->update();
    
    ftft.fillRect(16, 480-16-64-80, 16+64, 480-16-80, {64, 64, 64});
    ftft.fillRect(16+80, 480-16-64, 16+64+80, 480-16, {64, 64, 64});
    ftft.fillRect(16, 480-16-64, 16+64, 480-16, {64, 64, 64});
    
    ButtonHandler leftButton = ButtonHandler(
        BUTTON_L,
        []{
            digitalWrite(LED_R, HIGH);
            
            ftft.fillRect(16, 480-16-64, 16+64, 480-16, {255, 0, 0});
            
            --c;
            itoa(c, tu_counter->buf, 10);
            tu_counter->fg = { 224, 48, 32 };
            tu_counter->update();
        }, []{
            digitalWrite(LED_R, LOW);
            
            ftft.fillRect(16, 480-16-64, 16+64, 480-16, {64, 64, 64});
        }
    );
    ButtonHandler rightButton = ButtonHandler(
        BUTTON_R,
        []{
            digitalWrite(LED_Y, HIGH);
            
            ftft.fillRect(16+80, 480-16-64, 16+64+80, 480-16, {255, 255, 0});
            
            ++c;
            itoa(c, tu_counter->buf, 10);
            tu_counter->fg = { 32, 192, 64 };
            tu_counter->update();
        }, []{
            digitalWrite(LED_Y, LOW);
            
            ftft.fillRect(16+80, 480-16-64, 16+64+80, 480-16, {64, 64, 64});
        }
    );
    ButtonHandler middleButton = ButtonHandler(
        BUTTON_M,
        []{
            ftft.fillRect(16, 480-16-64-80, 16+64, 480-16-80, {0, 192, 0});
            
            c = 0;
            itoa(c, tu_counter->buf, 10);
            tu_counter->fg = { 64, 96, 255 };
            tu_counter->update();
        }, []{
            ftft.fillRect(16, 480-16-64-80, 16+64, 480-16-80, {64, 64, 64});
        }
    );
    
    leftButton.init();
    rightButton.init();
    middleButton.init();
    
    while (true) {
        leftButton.update();
        rightButton.update();
        middleButton.update();
        delay(19); // 🙏
    }
}
