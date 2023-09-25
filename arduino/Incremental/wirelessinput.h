//if secondary controller
//	handles pushed commands
//	Parse Input 
//	Apply Commands
//#include "Arduino.h"
#ifndef wirelessinput_h
#define wirelessinput_h

#ifndef CHANNEL
    #define CHANNEL 1
#endif

#include "parseinput.h"
#include <esp_now.h>
#include <WiFi.h>

class WirelessInput
{
public:
    WirelessInput(ParseInput *parseInput);
    void ConfigureListener();
    void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len);
private:
    ParseInput *_parseInput;
    void configDeviceAP();
    void InitESPNow();
};
#endif
