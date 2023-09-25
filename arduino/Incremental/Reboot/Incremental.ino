#include <stdlib.h>

// Custom Libraries
#include "lighting.h"
#include "commands.h"
#include "connection.h"
#include "pixellayout.h"

#define PRIMARY_CONTROLLER
#ifdef PRIMARY_CONTROLLER
  #define CONTROLLER_ID 0
#endif
#ifndef PRIMARY_CONTROLLER
  #define CONTROLLER_ID 1
#endif

#ifdef PRIMARY_CONTROLLER
  // For web hosting
  #include "WiFi.h"
  #include "ESPAsyncWebSrv.h"
  const char* ssid = "Crazy-Kitchen-Lights";
  const char* password = "forget13";
  AsyncWebServer server(80);

  // For display
  #include <SPI.h>
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  #define SCREEN_WIDTH 128
  #define SCREEN_HEIGHT 32
  #define OLED_RESET -1
  #define SCREEN_ADDRESS 0x3C
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

  // For ESP Now
  #include <esp_now.h>
  #include <WiFi.h>
  #ifdef PRIMARY_CONTROLLER
    #include <esp_wifi.h> // only for esp_wifi_set_channel()  
  #endif
  // Global copy of slave
  esp_now_peer_info_t slave;
  #define CHANNEL 1
  #ifdef PRIMARY_CONTROLLER
    #define PRINTSCANRESULTS 0
    #define DELETEBEFOREPAIR 0
  #endif

#endif

// Lights
#include <Adafruit_NeoPixel.h>
#define LED_PIN 4
Adafruit_NeoPixel strip(Controller[CONTROLLER_ID].end - Controller[CONTROLLER_ID].start, LED_PIN, NEO_GRBW + NEO_KHZ800);

// custom objects
unsigned long startMillis;
unsigned long currentMillis;
unsigned long lastCheckedEspNow;

String tempText="";
Lighting lighting = Lighting();
#ifdef PRIMARY_CONTROLLER
  Connection connection = Connection(&slave, (Stream*)&Serial);
#endif
#ifndef PRIMARY_CONTROLLER
  Connection connection = Connection(&(commands.process_request_string), &Serial);
#endif
Commands commands = Commands(&lighting, &connection, &Serial);

void setup() {
  startMillis = millis(); 
  lastCheckedEspNow = 0;
  
  Serial.begin(115200);
  //Serial.availableForWrite();
  Serial.println();
  Serial.println("Starting...");

  #ifdef PRIMARY_CONTROLLER
    // For ESP Now
    WiFi.mode(WIFI_STA);
    esp_wifi_set_channel(CHANNEL, WIFI_SECOND_CHAN_NONE);
    Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());
    Serial.print("STA CHANNEL "); Serial.println(WiFi.channel());
    command.InitESPNow();
    esp_now_register_send_cb(command.OnDataSent);

    // For web hosting
    Serial.print("Setting Access Point…");
    // Remove the password parameter, if you want the AP (Access Point) to be open
    //WiFi.softAP(ssid, password);
    WiFi.softAP(ssid);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("IP address: ");
    Serial.println(IP);

    // Write to OLED display

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
    }
    display.clearDisplay();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(ssid);
    display.print("IP: ");
    display.println(IP);
    display.display();

    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/plain", lighting.toString().c_str());
    });

    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/plain", lighting.displaySettings().c_str());
    });

    server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
      if (request->url() == "/process") {
        //if (!handleTest(request, data)) request->send(200, "text/plain", "false");
        String cmd = "";
        for(size_t i=0; i<len; i++){
          cmd += (char)data[i];
        }
        String s = commands.process_request_string(cmd);
        if (s != ""){
          request->send(500, "text/plain", s);
        }
        request->send(200, "text/plain", "processed commands: " + cmd);
      }
    });

    // Start server
    server.begin();
  #endif

  #ifndef PRIMARY_CONTROLLER
    esp_now_register_recv_cb(connection.OnDataRecv);
  #endif
  // for lights
  strip.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show(); // Turn OFF all pixels ASAP
}

void loop() {
  currentMillis = millis();

  if (currentMillis < startMillis) { startMillis = currentMillis; } // to prevent issues on rollover...
  
  commands.process_request_stream();

  if (millis() - startMillis> 100) // Temporary test loop
  {
    startMillis = millis(); 
    lighting.draw(millis());
  }

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
