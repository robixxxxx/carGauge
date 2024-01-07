#include <CanPacketManager.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_GC9A01A.h"
// #include "needle.cpp"
#include "math.h"
#define TFT_DC  16
#define TFT_CS 5
#define TFT_BL 19
// If display breakout has a backlight control pin, that can be defined here
// as TFT_BL. On some breakouts it's not needed, backlight is always on.


CanPacketManager packetManager;
// Display constructor for primary hardware SPI connection -- the specific
// pins used for writing to the display are unique to each board and are not
// negotiable. "Soft" SPI (using any pins) is an option but performance is
// reduced; it's rarely used, see header file for syntax if needed.
Adafruit_GC9A01A tft(TFT_CS, TFT_DC);

int Vertex0x = tft.width()/2-2;
int Vertex0y = tft.height()/2;

int Vertex1x = tft.width()/2+2;
int Vertex1y = tft.height()/2;

int end_x = tft.width()/2;
int end_y = tft.height()/2+100;

int i=0;
void drawNeedle(int angle, int length){
        tft.fillTriangle(Vertex0x, Vertex0y, Vertex1x, Vertex1y, end_x, end_y, GC9A01A_BLACK);
        tft.fillCircle(tft.width()/2, tft.height()/2, 5, GC9A01A_DARKGREY);
        Vertex0x = tft.width()/2 + 3 * cos(radians(90+angle));
        Vertex0y = tft.height()/2 - 3 * sin(radians(90+angle));
        Vertex1x = tft.width()/2 - 3 * cos(radians(90+angle));
        Vertex1y = tft.height()/2 + 3 * sin(radians(90+angle));
        end_x = tft.width()/2 + length * cos(radians(angle));
        end_y = tft.height()/2 - length * sin(radians(angle));
        // Narysuj wskazówkę
        tft.fillTriangle(Vertex0x, Vertex0y, Vertex1x, Vertex1y, end_x, end_y, GC9A01A_RED);
        tft.fillCircle(tft.width()/2, tft.height()/2, 5, GC9A01A_DARKGREY);
}

// needle ned = needle();
void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.fillScreen(GC9A01A_BLACK);
  yield();
}

void loop(void) {
  packetManager.CANsimulate();
  

  drawNeedle(i%360,100);
  i+=5;
  
  delay(20);
}
