// general platform setup
#include <Arduino.h>

// will be used to interface with our leds
#include <FastLED.h>

// our wifi and web server libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>

// our matrix helper
#include "matrix.hpp"

// some general matrix configuration
#define MATRIX_ORIENTATION MATRIX_ROT_180
#define MATRIX_PIN         6
#define MATRIX_ROWS        16
#define MATRIX_COLUMNS     16

// some wifi settings
#define WIFI_SSID "bacon_palace"
#define WIFI_PASS "baconbacon"

// this is where our fastled leds will live
CRGB leds[MATRIX_ROWS * MATRIX_COLUMNS];

// initialize our matrix interface
Matrix *matrix = new Matrix(
  MATRIX_ROWS,
  MATRIX_COLUMNS,
  leds,
  MATRIX_ORIENTATION
);

// our wifi connection details
const char* wifiSsid PROGMEM = WIFI_SSID;
const char* wifiPass PROGMEM = WIFI_PASS;

// this is where our webserver lives
AsyncWebServer server(80);

// special settings for our api server
#define MAX_JSON_BUFFER_SIZE 64000

// some web server helpers
void setupApiServer();

// setup the controller
void setup() {
  // register our leds
  FastLED.addLeds<NEOPIXEL, 25>(leds, MATRIX_ROWS * MATRIX_COLUMNS);

  // connect to the wifi (with auto reconnect)
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSsid, wifiPass);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  }
  WiFi.setAutoReconnect(true);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // setup our api server
  setupApiServer();
  server.begin();
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setupApiServer() {
  server.onNotFound(notFound);

  // accept our new pixel configuration as an json object
  AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/api/matrix", [](AsyncWebServerRequest *request, JsonVariant &json) {
    // get json
    auto jsonObj = json.as<JsonObject>();
    // check for brightness setting
    if(jsonObj.containsKey("brightness")) {
      // parse brightness
      auto brightness = jsonObj["brightness"].as<uint8_t>();
      FastLED.setBrightness(brightness);
    }
    // check for pixels
    if(jsonObj.containsKey("pixels")) {
      matrix->showPixel(15, 15, CRGB::Red);
      // see pixels value as array
      auto pixels = jsonObj["pixels"].as<JsonArray>();
      // iterate over all rows
      for(size_t y = 0; y < pixels.size(); y++) {
        // iterate through each row element
        for(size_t x = 0; x < pixels[y].size(); x++) {
          // parse color
          CRGB color(pixels[y][x].as<uint32_t>());
          // set pixel
          matrix->setPixel(x, y, color);
        }
      }
      // show updated matrix
      FastLED.show();
    }
    request->send(200, "text/plain", "success");
    return;
  }, MAX_JSON_BUFFER_SIZE);
  // add handler
  server.addHandler(handler);
}

void loop() {

}