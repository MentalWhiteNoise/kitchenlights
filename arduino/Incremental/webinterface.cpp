#include "webinterface.h"

WebInterface::WebInterface(ParseInput *parseInput, Display *display, AsyncWebServer *server, Lighting *lighting){
    _parseInput = parseInput;
    _display = display;
    _server = server;
    _lighting = lighting;
}
void WebInterface::ConfigureInterface(char* ssid, char* password){
    Serial.print("Setting Access Point…");
    // Remove the password parameter, if you want the AP (Access Point) to be open
    //WiFi.softAP(ssid, password);
    WiFi.softAP(ssid);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("IP address: ");
    Serial.println(IP);
    _display->SetLine(0, ssid);
    _display->SetLine(1, "IP: " + IP.toString());

    _server->on("/status", HTTP_GET, [&](AsyncWebServerRequest *request){
      request->send_P(200, "text/plain", _lighting->toString().c_str());
    });
    
    _server->on("/status", HTTP_GET, [&](AsyncWebServerRequest *request){
      request->send_P(200, "text/plain", WebInterface::_lighting->displaySettings().c_str());
    });

    _server->onRequestBody([&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
      if (request->url() == "/process") {
        //if (!handleTest(request, data)) request->send(200, "text/plain", "false");
        String cmd = "";
        for(size_t i=0; i<len; i++){
          cmd += (char)data[i];
        }
        String s = WebInterface::_parseInput->process_request_string(cmd);
        if (s != ""){
          request->send(500, "text/plain", s);
        }
        request->send(200, "text/plain", "processed commands: " + cmd);
      }
    });
    
    // Start server
    _server->begin();
}
