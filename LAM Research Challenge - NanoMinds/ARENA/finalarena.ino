#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "nanominds1.h"
#include "nanominds2.h"

// TFT Pins
#define TFT_CS   15
#define TFT_DC   13
#define TFT_RST  4
#define TFT_SCLK 18
#define TFT_MOSI 22

// IR Pins
#define IR1  25
#define IR2  26
#define IR3  27

// Relay Pins
#define RELAY1  33
#define RELAY2  32

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// Latch flags
bool trig1 = false;
bool trig2 = false;
bool trig3 = false;

void setup() {
  Serial.begin(115200);

  // Relay setup
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);

  // IR setup
  pinMode(IR1, INPUT_PULLUP);
  pinMode(IR2, INPUT_PULLUP);
  pinMode(IR3, INPUT_PULLUP);

  // TFT setup
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
}

void loop() {

  // -----------------------------
  // IR SENSOR 1 → reverse relay
  // -----------------------------
  if (!trig1) {
    if (digitalRead(IR1) == LOW) {
      digitalWrite(RELAY1, LOW);
      trig1 = true;
    }
  }

  // -----------------------------
  // IR SENSOR 2 → reverse relay
  // -----------------------------
  if (!trig2) {
    if (digitalRead(IR2) == LOW) {
      digitalWrite(RELAY2, LOW);
      trig2 = true;
    }
  }

  // --------------------------------------
  // IR SENSOR 3 → TFT animation system
  // --------------------------------------
  if (!trig3) {
    if (digitalRead(IR3) == LOW) {
      trig3 = true;

      // First message screen
      tft.fillScreen(ST77XX_BLACK);
      tft.setTextColor(ST77XX_WHITE);
      tft.setTextSize(2);
      tft.setCursor(10, 10);
      tft.println("Load Weight");
      tft.setCursor(10, 40);
      tft.println("Matched");

      delay(1500);

      tft.drawRGBBitmap(0, 0, nanominds1, nanominds1_width, nanominds1_height);
      delay(1500);

      tft.drawRGBBitmap(0, 0, nanominds2, nanominds2_width, nanominds2_height);
      delay(1500);
    }

    return; 
  }

  // ---------------------------------------------
  // After first trigger → infinite image loop
  // ---------------------------------------------
  tft.drawRGBBitmap(0, 0, nanominds1, nanominds1_width, nanominds1_height);
  delay(1200);

  tft.drawRGBBitmap(0, 0, nanominds2, nanominds2_width, nanominds2_height);
  delay(1200);
}
