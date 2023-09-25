#include "Arduino.h"
#include <WiFi.h>
#include <esp_now.h>
#ifndef connection_h
#define connection_h

#ifndef PRIMARY_CONTROLLER
  typedef String (*processRequestFunction)(String text);
#endif

class Connection
{
  public:
    #ifdef PRIMARY_CONTROLLER
      Connection(esp_now_peer_info_t *slave, Stream *serial);
    #endif
    #ifndef PRIMARY_CONTROLLER
      Connection(processRequestFunction fn, Stream *serial);
    #endif
    void InitESPNow();
    #ifdef PRIMARY_CONTROLLER
      void ScanForSlave();
      bool manageSlave();
      void deletePeer();
      void sendData(uint8_t *data, uint8_t sizeOfData);
      void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
    #endif
    #ifndef PRIMARY_CONTROLLER
      void configDeviceAP();
      void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len);
    #endif
  private:
    Stream *_serial;
    #ifdef PRIMARY_CONTROLLER
      esp_now_peer_info_t *_slave;
    #endif
    #ifndef PRIMARY_CONTROLLER
      processRequestFunction _fn;
    #endif
};
#endif