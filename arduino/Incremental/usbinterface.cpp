#include "usbinterface.h"

UsbInterface::UsbInterface(Stream *serial, ParseInput *parseInput){
    _serial = serial;
    _parseInput = parseInput;
}
void UsbInterface::CheckAndProcess(){
  // FUTURE: Add ability to "age out" if they did not send an end character.
  // If invalid, still should read to end of stream and clear out the bad request.
  String s = "";
  while (_serial->available()) { // how will this impact the timing?  Should I just check if serial is available, and then read it if it is? How about syncing the command?
    char c = _serial->read();  //gets one byte from serial buffer
    s += _parseInput->process_request_char(c);
  }
  if (s != "")
  { _serial->println(s); } // Add _display update... 

}
