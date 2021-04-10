#pragma once

#include <stdint.h>

#include <FastLED.h>

enum MatrixOrientation {
    MATRIX_ROT_0,
    MATRIX_ROT_180,
};

class Matrix {
    private:
        uint8_t Rows, Columns;
        CRGB *Leds;
        MatrixOrientation Orientation;

        uint16_t calculatePixelPosition(uint8_t x, uint8_t y);
    public:
        Matrix(
            uint8_t rows,
            uint8_t columns,
            CRGB *leds,
            MatrixOrientation orientation = MATRIX_ROT_0
            ) : Rows(rows), Columns(columns), Leds(leds), Orientation(orientation) {}
        void setPixel(uint8_t x, uint8_t y, CRGB color);
        void showPixel(uint8_t x, uint8_t y, CRGB color);
};