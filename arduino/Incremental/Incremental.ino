/* OUTSTANDING ITEMS

Concerns: 
  * Had some strange colors while testing chase, and chase never did go to the darker side. Maybe a bug with my merging of chase effects? Verify cycles from chase, and application of cycles to colors.

Next steps: 
  * Validate chase pattern / color blending. Might have just been the application of the sign wave, which I turned off...

  * Testing: 
    * Targets
    * Shifting    

  * Adding pixelate, flicker, alternate, and other random effects. Verify if flicker activation should control anything in any of the other modes.
  
  * Build out web page on touch screen arduino... Is there a way to connect one device to two wireless networks, so I can access the services from our wireless network?
*/


#include <stdlib.h>
#include <EEPROM.h> // For remembering state

#define LOAD_STATE // Disable for initial save... 
#define PRIMARY_CONTROLLER

// For web hosting
#ifdef PRIMARY_CONTROLLER
  #include "WiFi.h"
  #include "ESPAsyncWebSrv.h"
  AsyncWebServer server(80);
#endif

// For display
#ifdef PRIMARY_CONTROLLER
  #include <SPI.h>
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  #define SCREEN_WIDTH 128
  #define SCREEN_HEIGHT 32
  #define OLED_RESET -1

  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif

// For ESP Now
#include <esp_now.h>
#include <WiFi.h>
#ifdef PRIMARY_CONTROLLER
  #define BUTTON_PIN 18
  #include <esp_wifi.h> // only for esp_wifi_set_channel()  
  // Global copy of slave
  esp_now_peer_info_t slave;
#endif

#include "pixellayout.h"

// Lights
#include <Adafruit_NeoPixel.h>
#define LED_PIN 4
#ifdef PRIMARY_CONTROLLER
  #define CONTROLLER_ID 0
#endif
#ifndef PRIMARY_CONTROLLER
  #define CONTROLLER_ID 1
#endif

// Add two strips for secondary... 
Adafruit_NeoPixel strip(Controller[CONTROLLER_ID].end - Controller[CONTROLLER_ID].start, LED_PIN, NEO_GRBW + NEO_KHZ800);

// Custom Libraries
#include "lighting.h"
Lighting lighting = Lighting();

#include "parseinput.h"
#include "managelighting.h"
#ifdef PRIMARY_CONTROLLER
  #include "wirelesspush.h"
  Display screen = Display(&display); //Adafruit_SSD1306
  WirelessPush wirelessPush = WirelessPush(&screen, &slave);
  ManageLighting manageLighting = ManageLighting(&lighting, &wirelessPush, &screen, &strip, CONTROLLER_ID); // Adafruit_NeoPixel
  ParseInput parseInput = ParseInput(&manageLighting);
  #include "webinterface.h"
  WebInterface webInterface = WebInterface(&parseInput, &screen, &server, &lighting);
#else //#ifndef PRIMARY_CONTROLLER
  ManageLighting manageLighting = ManageLighting(&lighting, nullptr, nullptr, &strip, CONTROLLER_ID); // Adafruit_NeoPixel
  ParseInput parseInput = ParseInput(&manageLighting);
  #include "wirelessinput.h"
  WirelessInput wirelessInput = WirelessInput(&parseInput);
#endif

#include "usbinterface.h"
UsbInterface usbInterface = UsbInterface(&Serial, &parseInput);

// custom objects
unsigned long startMillis;
unsigned long currentMillis;
unsigned long lastCheckedEspNow;

String tempText="";

#ifndef PRIMARY_CONTROLLER
// THIS SHOULD NOT BE REQUIRED!!! WHAT THE HELL?????
void OnDataReceive(const uint8_t *mac_addr, const uint8_t *data, int data_len){
  Serial.println("Data came in...");
  wirelessInput.OnDataRecv(mac_addr, data, data_len);
}
#endif
void setup() {

  EEPROM.begin(92);
  delay(500);
  
  startMillis = millis(); 
  lastCheckedEspNow = 0;
  
  Serial.begin(115200);
  //Serial.availableForWrite();
  Serial.println();
  Serial.println("Starting...");

  /*
  testChaseEffects("BOUNCE");
  delay(10000);
  testChaseEffects("OUTWARD");
  delay(10000);
  testChaseEffects("INWARD");
  delay(10000);
  testChaseEffects("CENTERBOUNCE");
  delay(10000);
  testChaseEffects("CLOCKWISE");
  delay(10000);
  testChaseEffects("COUNTER");
  delay(10000);*/

  #ifdef PRIMARY_CONTROLLER
    screen.StartDisplay();
    wirelessPush.ConfigureConnection();
    webInterface.ConfigureInterface("Crazy-Kitchen-Lights", "forget13");
    pinMode(BUTTON_PIN, INPUT_PULLUP);
  #else
    wirelessInput.ConfigureListener();
    esp_now_register_recv_cb(OnDataReceive); // THIS SHOULD NOT BE REQUIRED!!! WHAT THE HELL?????
  #endif
 
  // for lights
  strip.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show(); // Turn OFF all pixels ASAP


  #ifdef LOAD_STATE
    //delay(2000);
    Serial.print("should load");
    manageLighting.load();
  #endif
}
#include "lightingchase.h"
#include "pixellayout.h"

void loop() {
  currentMillis = millis();

  //if (currentMillis < startMillis) { startMillis = currentMillis; } // to prevent issues on rollover...

  usbInterface.CheckAndProcess();
  #ifdef PRIMARY_CONTROLLER
  screen.draw(currentMillis);
  if(digitalRead(BUTTON_PIN) == LOW){
    screen.wake();
  }
  #endif
  manageLighting.draw(currentMillis);

  // If changes were made, wait unitl X amount of time has passed after the last change to save to EEPROM... (limit writes) OR add a save command!
  // EEPROM on Arduino Mega is 4096 bytes... 
  // https://docs.arduino.cc/learn/built-in-libraries/eeprom

  /* Draw should look something like:
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    //strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue))); // what's this gamma stuff?
    // strip.Color(0, 0, 0, strip.gamma8(j))...
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }*/
}
