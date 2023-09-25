#include "wirelessinput.h"

WirelessInput::WirelessInput(ParseInput *parseInput){
    _parseInput = parseInput;
}
void WirelessInput::ConfigureListener(){
    WiFi.mode(WIFI_AP);
    configDeviceAP();
    Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());
    InitESPNow();
    //esp_now_register_recv_cb(OnDataRecv); // Can't register here as a void(*) function cannot be sent from a lambda... and statics aparently break everything...
}

void WirelessInput::OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len){
    String cmd = "";
    for(int i = 0; i < data_len; ++i){
        cmd += (char)data[i];
    }
    String s = _parseInput->process_request_string(cmd);
    if (s != ""){
        Serial.println(s);
    }
    Serial.println("Request processed: " + cmd);
}
void WirelessInput::configDeviceAP() {
  const char *SSID = "Slave_1";
  bool result = WiFi.softAP(SSID, "Slave_1_Password", CHANNEL, 0);
  if (!result) {
    Serial.println("AP Config failed.");
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
    Serial.print("AP CHANNEL "); Serial.println(WiFi.channel());
  }
}
void WirelessInput::InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    ESP.restart();
  }
}