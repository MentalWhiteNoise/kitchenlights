# include "connection.h"

#ifndef PRIMARY_CONTROLLER
  #define CHANNEL 1
#endif
#ifdef PRIMARY_CONTROLLER
Connection::Connection(esp_now_peer_info_t *slave, Stream *serial){
  _serial = serial;
  _slave = slave;
}
#endif
#ifndef PRIMARY_CONTROLLER
Connection::Connection(processRequestFunction fn, Stream *serial){
  _serial = serial;
  _fn = fn;
}
#endif
void Connection::InitESPNow(){
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    _serial->println("ESPNow Init Success");
  }
  else {
    _serial->println("ESPNow Init Failed");
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    ESP.restart();
  }
}
#ifdef PRIMARY_CONTROLLER
  void Connection::ScanForSlave(){
    int16_t scanResults = WiFi.scanNetworks(false, false, false, 300, CHANNEL); // Scan only on one channel
    // reset on each scan
    bool slaveFound = 0;
    memset(_slave, 0, sizeof(esp_now_peer_info_t)); // do I need to write this to memory? was &slave...

    _serial->println("");
    if (scanResults == 0) {
      _serial->println("No WiFi devices in AP Mode found");
    } else {
      _serial->print("Found "); _serial->print(scanResults); _serial->println(" devices ");
      for (int i = 0; i < scanResults; ++i) {
        // Print SSID and RSSI for each device found
        String SSID = WiFi.SSID(i);
        int32_t RSSI = WiFi.RSSI(i);
        String BSSIDstr = WiFi.BSSIDstr(i);

        if (PRINTSCANRESULTS) {
          _serial->print(i + 1);
          _serial->print(": ");
          _serial->print(SSID);
          _serial->print(" (");
          _serial->print(RSSI);
          _serial->print(")");
          _serial->println("");
        }
        delay(10);
        // Check if the current device starts with `Slave`
        if (SSID.indexOf("Slave") == 0) {
          // SSID of interest
          _serial->println("Found a Slave.");
          _serial->print(i + 1); 
          _serial->print(": "); 
          _serial->print(SSID); 
          _serial->print(" ["); 
          _serial->print(BSSIDstr); 
          _serial->print("]"); 
          _serial->print(" ("); 
          _serial->print(RSSI); 
          _serial->print(")"); 
          _serial->println("");
          // Get BSSID => Mac Address of the Slave
          int mac[6];
          if ( 6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5] ) ) {
            for (int ii = 0; ii < 6; ++ii ) {
              _slave->peer_addr[ii] = (uint8_t) mac[ii];
            }
          }

          _slave->channel = CHANNEL; // pick a channel
          _slave->encrypt = 0; // no encryption

          slaveFound = 1;
          // we are planning to have only one slave in this example;
          // Hence, break after we find one, to be a bit efficient
          break;
        }
      }
    }

    if (slaveFound) {
      _serial->println("Slave Found, processing..");
    } else {
      _serial->println("Slave Not Found, trying again.");
    }

    // clean up ram
    WiFi.scanDelete();
  }
  bool Connection::manageSlave(){
    if (_slave->channel == CHANNEL) {
      if (DELETEBEFOREPAIR) {
        deletePeer();
      }

      _serial->print("Slave Status: ");
      // check if the peer exists
      bool exists = esp_now_is_peer_exist(slave.peer_addr);
      if ( exists) {
        // Slave already paired.
        _serial->println("Already Paired");
        return true;
      } else {
        // Slave not paired, attempt pair
        esp_err_t addStatus = esp_now_add_peer(_slave); // was &slave...
        if (addStatus == ESP_OK) {
          // Pair success
          _serial->println("Pair success");
          return true;
        } else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT) {
          // How did we get so far!!
          _serial->println("ESPNOW Not Init");
          return false;
        } else if (addStatus == ESP_ERR_ESPNOW_ARG) {
          _serial->println("Invalid Argument");
          return false;
        } else if (addStatus == ESP_ERR_ESPNOW_FULL) {
          _serial->println("Peer list full");
          return false;
        } else if (addStatus == ESP_ERR_ESPNOW_NO_MEM) {
          _serial->println("Out of memory");
          return false;
        } else if (addStatus == ESP_ERR_ESPNOW_EXIST) {
          _serial->println("Peer Exists");
          return true;
        } else {
          _serial->println("Not sure what happened");
          return false;
        }
      }
    } else {
      // No slave found to process
      _serial->println("No Slave found to process");
      return false;
    }
    
  }
  void Connection::deletePeer(){
    esp_err_t delStatus = esp_now_del_peer(slave.peer_addr);
    _serial->print("Slave Delete Status: ");
    if (delStatus == ESP_OK) {
      // Delete success
      _serial->println("Success");
    } else if (delStatus == ESP_ERR_ESPNOW_NOT_INIT) {
      // How did we get so far!!
      _serial->println("ESPNOW Not Init");
    } else if (delStatus == ESP_ERR_ESPNOW_ARG) {
      _serial->println("Invalid Argument");
    } else if (delStatus == ESP_ERR_ESPNOW_NOT_FOUND) {
      _serial->println("Peer not found.");
    } else {
      _serial->println("Not sure what happened");
    }
  }
  void Connection::sendData(uint8_t *data, uint8_t sizeOfData){
    const uint8_t *peer_addr = _slave->peer_addr;
    _serial->print("Sending: "); _serial->println(data);
    esp_err_t result = esp_now_send(peer_addr, data, sizeOfData);
    _serial->print("Send Status: ");
    if (result == ESP_OK) {
      _serial->println("Success");
    } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
      // How did we get so far!!
      _serial->println("ESPNOW not Init.");
    } else if (result == ESP_ERR_ESPNOW_ARG) {
      _serial->println("Invalid Argument");
    } else if (result == ESP_ERR_ESPNOW_INTERNAL) {
      _serial->println("Internal Error");
    } else if (result == ESP_ERR_ESPNOW_NO_MEM) {
      _serial->println("ESP_ERR_ESPNOW_NO_MEM");
    } else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
      _serial->println("Peer not found.");
    } else {
      _serial->println("Not sure what happened");
    }
  }
  void Connection::OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
            mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    _serial->print("Last Packet Sent to: "); _serial->println(macStr);
    _serial->print("Last Packet Send Status: "); _serial->println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");

  }
#endif
#ifndef PRIMARY_CONTROLLER
  void Connection::configDeviceAP(){
    const char *SSID = "Slave_1";
    bool result = WiFi.softAP(SSID, "Slave_1_Password", CHANNEL, 0);
    if (!result) {
      _serial->println("AP Config failed.");
    } else {
      _serial->println("AP Config Success. Broadcasting with AP: " + String(SSID));
      _serial->print("AP CHANNEL "); _serial->println(WiFi.channel());
    }
  }
  void Connection::OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len){
    String cmd = "";
    for(int i = 0; i < data_len; ++i){
      cmd += (char)data[i];
    }
    String s = _fn(cmd);
    if (s != ""){
      _serial->println(s);
    }
    _serial->println("Request processed: " + cmd);
  }
#endif