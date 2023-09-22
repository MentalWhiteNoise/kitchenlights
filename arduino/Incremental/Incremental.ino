#include <stdlib.h>
#include "lighting.h"
#include "commands.h"

unsigned long startMillis;
unsigned long currentMillis;

String tempText="";
Lighting lighting = Lighting();
Commands commands = Commands(&lighting, &Serial);

void setup() {
  //pinMode(redPin, OUTPUT);
  //pinMode(bluePin, OUTPUT);
  //pinMode(greenPin, OUTPUT);
  //SetColor(0);
  startMillis = millis(); 
  
  Serial.begin(115200);
  //Serial.availableForWrite();
  Serial.println();
  Serial.println("Starting...");
}

void loop() {
  currentMillis = millis();

  if (currentMillis < startMillis) { startMillis = currentMillis; } // to prevent issues on rollover...
  
  commands.process_request_stream();

  if (millis() - startMillis> 100)
  {
    startMillis = millis(); 
    lighting.draw(millis());
  }

  // If changes were made, wait unitl X amount of time has passed after the last change to save to EEPROM... (limit writes) OR add a save command!
  // EEPROM on Arduino Mega is 4096 bytes... 
  // https://docs.arduino.cc/learn/built-in-libraries/eeprom

}
