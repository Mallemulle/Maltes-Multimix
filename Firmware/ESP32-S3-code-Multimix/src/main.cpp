#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <U8g2_for_Adafruit_GFX.h>
#include <Fonts/FreeSerifBold12pt7b.h>
#include <USB.h>
#include <USBHIDKeyboard.h>
#include <USBHIDConsumerControl.h>
#include <Wire.h>



// Creates objects so computer sees device as keyboard and media controller
USBHIDKeyboard Keyboard;
USBHIDConsumerControl Consumer;


// Screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// I2C pins for ESP32-S3
#define I2C_SDA 13
#define I2C_SCL 14

// Media push buttons
#define PREV_TRACK 45
#define PLAY_PAUSE 38
#define NEXT_TRACK 2
int lastPrevState = 0;
int lastPlayState = 0;
int lastNextState = 0;

// Button debouncing
#define BUTTON_DEBOUNCE 50 // 50 ms
unsigned long debouncePrev = 0;
unsigned long debouncePlay = 0;
unsigned long debounceNext = 0;

// KY-040 rotary encoder
#define KY040_SW 5
#define KY040_DT 6
#define KY040_CLK 47

// Display object declaration
#define OLED_RESET -1 // No reset pin
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/* Writes and centers text on OLED
Inparameters: text (string)
Returns: None */
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

/* Checks if button is pressed and wasn't pressed previous loop
Inparameters: buttonPin (int)
Returns: buttonPressed (bool) */
bool isButtonPressed(int buttonPin, int &lastState, unsigned long &lastDebounce) {
  bool buttonPressed = false;
  int buttonState = digitalRead(buttonPin);

  unsigned long timeSincePress = millis() - lastDebounce;
  if (buttonState == HIGH && lastState == LOW && timeSincePress > BUTTON_DEBOUNCE) {
    buttonPressed = true;
    lastDebounce = millis();
  }

  lastState = buttonState;
  return buttonPressed;
}


void setup() {
  Serial.begin(115200); // initialize Serial Monitor
  
  // Start built in USB services
  Keyboard.begin();
  Consumer.begin();
  USB.begin();

  // Buttons
  pinMode(PREV_TRACK, INPUT_PULLDOWN);
  pinMode(PLAY_PAUSE, INPUT_PULLDOWN);
  pinMode(NEXT_TRACK, INPUT_PULLDOWN);
  
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
}

void loop() {
  // Media buttons
  // Previous Track
  if (isButtonPressed(PREV_TRACK, lastPrevState, debouncePrev)) {
    Consumer.press(CONSUMER_CONTROL_SCAN_PREVIOUS);
    Consumer.release();
  }
  // Play/Pause
  if (isButtonPressed(PLAY_PAUSE, lastPlayState, debouncePlay)) {
    Consumer.press(CONSUMER_CONTROL_PLAY_PAUSE);
    Consumer.release();
  }
  // Next Track
  if (isButtonPressed(NEXT_TRACK, lastNextState, debounceNext)) {
    Consumer.press(CONSUMER_CONTROL_SCAN_NEXT);
    Consumer.release();
  }
}