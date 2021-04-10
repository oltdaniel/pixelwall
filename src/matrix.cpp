#include "matrix.hpp"

uint16_t Matrix::calculatePixelPosition(uint8_t x, uint8_t y) {
    uint16_t base;
    switch (Orientation) {
    case MATRIX_ROT_0:
        base = (x * Rows);
        if(x % 2 == 1) {
            base += Rows - y - 1;
        } else {
            base += y;
        }
        return base;
    case MATRIX_ROT_180:
        base = (x * Rows);
        if(x % 2 == 0) {
            base += Rows - y;
        } else {
            base += y + 1;
        }
        return (Rows * Columns) - base;
    }
    return 0;
}

void Matrix::setPixel(uint8_t x, uint8_t y, CRGB color) {
    Leds[calculatePixelPosition(x, y)] = color;
}

void Matrix::showPixel(uint8_t x, uint8_t y, CRGB color) {
    setPixel(x, y, color);
    FastLED.show();
}