#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <U8g2_for_Adafruit_GFX.h>
#include <Fonts/FreeSerifBold12pt7b.h>
#include <Wire.h>

// Screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// I2C pins for ESP32-S3
#define I2C_SDA 13
#define I2C_SCL 14

// Push button
// #define buttonPin 13
// int buttonState = 0;
// int lastButtonState = 0;

// Display object declaration
#define OLED_RESET -1 // No reset pin
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Writes and centers text on OLED
void writeText(String text) {
  int16_t x1, y1;
  uint16_t w, h;

  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

  int16_t x = (SCREEN_WIDTH - w) / 2;
  int16_t y = (SCREEN_HEIGHT - h) / 2;

  display.setCursor(x, y);
  display.print(text);
  display.display(); // pushes to screen
}


void setup() {
  Serial.begin(115200); // initialize Serial Monitor
  Wire.begin(I2C_SDA, I2C_SCL);

  delay(1000);

  // Initialize display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true) {
      ; // Loop forever if screen fails
    }
  }

  display.clearDisplay();
  display.setFont(&FreeSerifBold12pt7b);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);


  String text = "Universal";
  writeText(text);

  // pinMode(buttonPin, INPUT_PULLDOWN);
}

void loop() {
  // buttonState = digitalRead(buttonPin);
  // if (buttonState == HIGH && buttonState != lastButtonState) {
    
  // }

  // lastButtonState = buttonState;
  // test
}