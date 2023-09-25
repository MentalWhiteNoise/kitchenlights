// Handles USB Input
// Parse Input 
// Apply Commands

//#include "Arduino.h"
#ifndef usbinterface_h
#define usbinterface_h

#include "parseinput.h"

class UsbInterface
{
public:
    UsbInterface(Stream *serial, ParseInput *parseInput);
    void CheckAndProcess();
private:
    Stream *_serial;
    ParseInput *_parseInput;
};
#endif
