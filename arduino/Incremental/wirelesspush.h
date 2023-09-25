//if primary controller
//	Handles pushing commands

//#include "Arduino.h"
#ifndef wirelesspush_h
#define wirelesspush_h

#ifndef CHANNEL
    #define CHANNEL 1
#endif
#define PRINTSCANRESULTS 0
#define DELETEBEFOREPAIR 0

#include <esp_now.h>
#include <WiFi.h>
#include "display.h"
#include <esp_wifi.h>

class WirelessPush
{
public:
    WirelessPush(Display *display, esp_now_peer_info_t *slave);
    void ConfigureConnection();
    void PushCommand(String command, String parameters);
private:
    Display *_display;
    esp_now_peer_info_t *_slave;
    void InitESPNow();
    void ScanForSlave();
    bool manageSlave();
    void deletePeer();
    void sendData(uint8_t *data, uint8_t sizeOfData);
    static void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
};
#endif
