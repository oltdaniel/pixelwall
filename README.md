# pixelwall

a small pixel wall element project

## Hardware

I used an WS2812B LED Strip (5m, 60LEDs/m) to created an 16x16 Pixel Matrix. It is currently driven by an ESP32 Development Board, which I will replace with an
"raw" ESP32-WROOM Module in the future. This code is the initial prototype of the API to drive this controller. The data line is wired in a zig-zag schematic.

## API

Currently we have only 1 Endpoint, `/api/matrix` on port `80`. It only accepts JSON (needs to be explictly marked via `Content-Type: application/json` header). It accepts the following data:
```json
{
    "brightness": <value from 0 to 255>,
    "pixels: [
        [<color>, <color>, ...],
        ....
    ]
}
```
The brightness value will be directly put into [`FastLED.setBrightness`](http://fastled.io/docs/3.1/class_c_fast_l_e_d.html#a730ba7d967e882b4b893689cf333b2eb) as an `uint8_t`.
Each color is accepted as `r << 16 | b << 8 | g` combined into an `uint32_t` with each color part being `uint8_t`. You can send as many pixels, as your matrix supports, which means less
pixels is possible. Currently there is no error handling included.

## Configuration

In the first half of `src/main.cpp` you will currently find the configuration options:
```c
// some general matrix configuration
#define MATRIX_ORIENTATION MATRIX_ROT_180 // this can be MATRIX_ROT_180 or MATRIX_ROT_0
#define MATRIX_PIN         6              // the data in to the matrix
#define MATRIX_ROWS        16
#define MATRIX_COLUMNS     16

// some wifi settings
#define WIFI_SSID "bacon_palace"
#define WIFI_PASS "baconbacon"
```

Platform IO Stuff. Just make sure you have the correct settings in your `platformio.ini`.

> **NOTE**: Documentation is still work-in-progress.

## LICENSE

Hack it until you have what you want. MIT.