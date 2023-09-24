//Digole Digital Solutions: www.digole.com
#ifndef DigoleSerialDisp_h
#define DigoleSerialDisp_h

#include <inttypes.h>
#include "Print.h"
#include <Wire.h>
#include "Arduino.h"
#include <string.h>
// Communication set up command
// Text function command
// Graph function command
#define Serial_UART 0
#define Serial_I2C 1
#define Serial_SPI 2
#define _TEXT_ 0
#define _GRAPH_ 1
#ifdef FLASH_CHIP   //if writing to flash chip
#define _WRITE_DELAY 40
#else   //if writing to internal flash
#define _WRITE_DELAY 100
#endif
#ifndef Ver
#define Ver 40
#endif

class Digole {
public:

    virtual size_t read1(void);
    virtual void cleanBuffer(void);
    virtual void write2B(unsigned int);
    //    virtual int readInt(void);
};

class DigoleSerialDisp : public Print, public Digole {
public:
#if defined(_Digole_Serial_UART_)||(!defined(_Digole_Serial_I2C_)&&!defined(_Digole_Serial_SPI_))

    DigoleSerialDisp(HardwareSerial *s, unsigned long baud) //UART set up
    {
        _mySerial = s;
        _Baud = baud;
    }

    size_t write(uint8_t value) {
        _mySerial->write((uint8_t) value);
        return 1; // assume sucess
    }

    void begin(void) {
        _mySerial->begin(_Baud); //for auto baud display
        delay(100);
        _mySerial->print('U');
        delay(100);
        _mySerial->begin(9600);
        _mySerial->print("SB");
        _mySerial->println(_Baud);
        delay(100);
        _mySerial->begin(_Baud);
    }

    size_t read1(void) {
        int t;
        //        wdt_disable();
        do {
            t = _mySerial->read();
        } while (t == -1);
        return t;
    }

    void cleanBuffer(void) //clean UART receiving buffer
    {
        while (_mySerial->read() != -1);
    }
#ifdef _Screen_Over_511_
//for 4.3" high definition old module before 2023-07-07 only
void write2B(unsigned int v) {
        write(v / 256);
        write(v);
}
#else
    void write2B(unsigned int v) {  //for all screen
        if (v < 255)
            write(v);
        else {
            write(255);
            if((v-255)<240)
                write(v - 255);
            else
            {
                write(240+v/256);
                write(v%256);
            }
        }
    }

#endif
#endif
#if defined(_Digole_Serial_I2C_)

    void begin(void) {
        _myWire->begin();
    }

    DigoleSerialDisp(TwoWire *s, uint8_t addr) //U2C set up
    {
        _myWire = s;
        _I2Caddress = addr;
    }

    size_t write(uint8_t value) {
        _myWire->beginTransmission(_I2Caddress);
        _myWire->write((uint8_t) value); //use yourself routing to send bulk of data, such as for…
        _myWire->endTransmission();
        return 1; // assume sucess
    }

    size_t read1(void) {
        int t;
        //        wdt_disable();
        while(_myWire->requestFrom(_I2Caddress, (uint8_t)1)==0) ;
        if(_myWire->available() == 0) return -1;
        t = _myWire->read(); //use yourself routing to read bulk of data, such as for…
        return t; // assume sucess
    }

    void cleanBuffer(void) {
    }
#ifdef _Screen_Over_511_
//for 4.3" high definition old module before 2023-07-07 only
void write2B(unsigned int v) {
        write(v / 256);
        write(v);
}
#else
    void write2B(unsigned int v) {  //for all screen
        if (v < 255)
            write(v);
        else {
            write(255);
            if((v-255)<240)
                write(v - 255);
            else
            {
                write(240+v/256);
                write(v%256);
            }
        }
    }

#endif
#endif
#if defined(_Digole_Serial_SPI_)

    void begin(void) {
        pinMode(_Clockpin, OUTPUT);
        pinMode(_Datapin, OUTPUT);
        pinMode(_SSpin, OUTPUT);
        digitalWrite(_SSpin, HIGH);
#if Ver==33 //sorry, we made wrong mode in Ver 3.3 firmware
        digitalWrite(_Clockpin, HIGH);
#else
        digitalWrite(_Clockpin, LOW);
#endif
        digitalWrite(_Datapin, LOW);
        if (_SIpin != 0) {
            pinMode(_SIpin, INPUT);
        }
    }

    DigoleSerialDisp(uint8_t pin_data, uint8_t pin_clock, uint8_t SS, uint8_t SI = 0) //spi set up
    {
        _Clockpin = pin_clock;
        _Datapin = pin_data;
        _SSpin = SS;
        _SIpin = SI;
        begin();
        /*        pinMode(_Clockpin, OUTPUT);
                pinMode(_Datapin, OUTPUT);
                pinMode(_SSpin, OUTPUT);
                digitalWrite(_SSpin, HIGH);
        #if Ver==33 //sorry, we made wrong mode in Ver 3.3 firmware
                digitalWrite(_Clockpin, HIGH);
        #else
                digitalWrite(_Clockpin, LOW);
        #endif
                digitalWrite(_Datapin, LOW);
                if (_SIpin != 0) {
                    pinMode(_SIpin, INPUT);
                }
         */
    }

    size_t write(uint8_t value) {
        digitalWrite(_SSpin, LOW);
        digitalWrite(_SSpin, LOW);
        digitalWrite(_SSpin, LOW);
        shiftOut(_Datapin, _Clockpin, MSBFIRST, value);
#if Ver==33
        digitalWrite(_Clockpin, HIGH);
#endif
        digitalWrite(_SSpin, HIGH);
        return 1; // assume sucess
    }

    size_t read1(void) {
        int t = 0;
        char c;
        //        wdt_disable();
	digitalWrite(_Datapin,LOW);	//tell display this is reading, otherwise all pending data canceled
        digitalWrite(_SSpin, HIGH);
        digitalWrite(_SSpin, HIGH);
        while (digitalRead(_SIpin) == LOW) delay(1); //check to see the data is ready(1) or not(0)
        digitalWrite(_SSpin, LOW); //tell display module I will read data
        digitalWrite(_SSpin, LOW); //delay about 5us to wait module prepare data
        digitalWrite(_SSpin, LOW);
        for (c = 8; c > 0; c = c - 1) {
            t <<= 1;
#if Ver==33
            digitalWrite(_Clockpin, LOW);
            if (digitalRead(_SIpin))
                t |= 1;
            digitalWrite(_Clockpin, HIGH);
#else
            digitalWrite(_Clockpin, HIGH);
            if (digitalRead(_SIpin))
                t |= 1;
            digitalWrite(_Clockpin, LOW);
#endif
        }

        //	t=shiftIn(_Datapin, _Clockpin, MSBFIRST);
        digitalWrite(_SSpin, HIGH);
        return t; // assume sucess
    }

    void cleanBuffer(void) {
    }
#ifdef _Screen_Over_511_
//for 4.3" high definition old module before 2023-07-07 only
void write2B(unsigned int v) {
        write(v / 256);
        write(v);
}
#else
    void write2B(unsigned int v) {  //for all screen
        if (v < 255)
            write(v);
        else {
            write(255);
            if((v-255)<240)
                write(v - 255);
            else
            {
                write(240+v/256);
                write(v%256);
            }
        }
    }
#endif
#endif
    //print function

    void println(const __FlashStringHelper *v) {
        preprint();
        Print::println(v);
        write((uint8_t) 0);
        //        writeStr("TRT");
    }

    void println(const String &v) {
        preprint();
        Print::println(v);
        write((uint8_t) 0);
        //        writeStr("TRT");
    }

    void println(const char v[]) {
        preprint();
        Print::println(v);
        write((uint8_t) 0);
        //        writeStr("TRT");
    }

    void println(char v) {
        preprint();
        Print::println(v);
        write((uint8_t) 0);
        //        Print::println("\x0dTRT");
    }

    void println(unsigned char v, int base = 10) {
        preprint();
        Print::println(v, base);
        write((uint8_t) 0);
        //        Print::println("\x0dTRT");
    }

    void println(int v, int base = 10) {
        preprint();
        Print::println(v, base);
        write((uint8_t) 0);
        //        writeStr("TRT");
    }

    void println(unsigned int v, int base = 10) {
        preprint();
        Print::println(v, base);
        write((uint8_t) 0);
        //        writeStr("TRT");
    }

    void println(long v, int base = 10) {
        preprint();
        Print::println(v, base);
        write((uint8_t) 0);
        //        writeStr("TRT");
    }

    void println(unsigned long v, int base = 10) {
        preprint();
        Print::println(v, base);
        write((uint8_t) 0);
        //        writeStr("TRT");
    }

    void println(double v, int base = 2) {
        preprint();
        Print::println(v, base);
        write((uint8_t) 0);
        //        writeStr("TRT");
    }

    void println(const Printable& v) {
        preprint();
        Print::println(v);
        write((uint8_t) 0);
        //        writeStr("TRT");
    }

    void println(void) {
        //        write((uint8_t) 0);
        writeStr("TRT");
    }

    void print(const __FlashStringHelper *v) {
        preprint();
        Print::print(v);
        write((uint8_t) 0);
    }

    void print(const String &v) {
        preprint();
        Print::print(v);
        write((uint8_t) 0);
    }

    void print(const char v[]) {
        preprint();
        Print::print(v);
        write((uint8_t) 0);
    }

    void print(char v) {
        preprint();
        Print::print(v);
        write((uint8_t) 0);
    }

    void print(unsigned char v, int base = 10) {
        preprint();
        Print::print(v, base);
        write((uint8_t) 0);
    }

    void print(int v, int base = 10) {
        preprint();
        Print::print(v, base);
        write((uint8_t) 0);
    }

    void print(unsigned int v, int base = 10) {
        preprint();
        Print::print(v, base);
        write((uint8_t) 0);
    }

    void print(long v, int base = 10) {
        preprint();
        Print::print(v, base);
        write((uint8_t) 0);
    }

    void print(unsigned long v, int base = 10) {
        preprint();
        Print::print(v, base);
        write((uint8_t) 0);
    }

    void print(double v, int base = 2) {
        preprint();
        Print::print(v, base);
        write((uint8_t) 0);
    }

    void print(const Printable& v) {
        preprint();
        Print::print(v);
        write((uint8_t) 0);
    }

    void wakeUp(void) {
        write((uint8_t) 0);
        delay(100);
    }

    /*---------fucntions for Text and Graphic LCD adapters---------*/
    void writeStr(const char *s);
    void disableCursor(void);
    void enableCursor(void);
    void clearScreen(void);
    void setI2CAddress(uint8_t add);
    void displayConfig(uint8_t v);

    void setPushup(char v);
    void setTrans(char v);
    void setTextAlignment(char v);
    void preprint(void);
    int readInt(void);
    void write24b(uint32_t d);
    /*----------Functions for Graphic LCD/OLED adapters only---------*/
    //the functions in this section compatible with u8glib
    void invert(unsigned char onoff);	//for mono display only, set screen invert or not
    void drawBitmap(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap);
    void drawBitmap256(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap);
    void drawBitmap65K(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap);
    void drawBitmap262K(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap);
    void drawColorImg(unsigned char format, unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap);
    void setTrueColor(uint8_t r, uint8_t g, uint8_t b);
    void setRot90(void);
    void setRot180(void);
    void setRot270(void);
    void undoRotation(void);
    void setRotation(uint8_t);
    void setContrast(uint8_t);
    void drawBox(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
    void drawCircle(unsigned int x, unsigned int y, unsigned int r, uint8_t = 0);
    void drawDisc(unsigned int x, unsigned int y, unsigned int r);
    void drawFrame(unsigned int x, unsigned int y, unsigned int x1, unsigned int y1);
    //void drawPixel(unsigned int x, unsigned int y, uint8_t color);
    void drawPixel(unsigned int x, unsigned int y);
    void drawPixel(unsigned int x, unsigned int y, uint8_t color);
    void drawLine(unsigned int x, unsigned int y, unsigned int x1, unsigned int y1);
    void drawLineTo(unsigned int x, unsigned int y);
    void drawHLine(unsigned int x, unsigned int y, unsigned int w);
    void drawVLine(unsigned int x, unsigned int y, unsigned int h);
    //-------------------------------
    //special functions for our adapters
    //void uploadStartScreen(int lon, const unsigned char *data); //upload start screen
    void setFont(uint8_t font); //set font, availale: 6,10,18,51,120,123, user font: 200-203
    void nextTextLine(void); //got to next text line, depending on the font size
    void setColor(uint8_t); //set color for graphic function
    void backLightOn(void); //Turn on back light
    void backLightOff(void); //Turn off back light
    void screenOnOff(uint8_t); //turn screen on/off
    void mcuOff(void); //put MCU in sleep, it will wake up when new data received
    void cpuOff(void); //put MCU in sleep, it will wake up when new data received
    void moduleOff(void); //put whole module in sleep: Back light off, screen in sleep, MCU in sleep
    void backLightBrightness(uint8_t); //set backlight brightness,0~100

    void directCommand(uint8_t d); //send command to LCD drectly
    void directData(uint8_t d); //send data to LCD drectly
    void moveArea(unsigned int x0, unsigned int y0, unsigned int w, unsigned int h, char xoffset, char yoffset); //move a area of screen to another place
    void drawStr(unsigned int x, unsigned int y, const char *s);
    void setPrintPos(unsigned int x, unsigned int y, uint8_t = 0);
    void moveTo(int x, int y);
    void setLCDColRow(uint8_t col, uint8_t row);
    void setTextPosAbs(unsigned int x, unsigned int y);
    /*-----Touch screen functions---*/
    void calibrateTouchScreen(void);
    void readTouchInst(void);
    void readTouchPres(void);
    void readTouchScreen(void);
    void readTouchScreen(int *x, int *y);
    void readClick(void);
    void readClick(int *x, int *y);
    int readBattery(void);
    int readAux(void);
    int readTemperature(void);

    /*-----Flash memory functions---*/
    void flashErase(unsigned long int addr, unsigned long int length);
    void flashReadStart(unsigned long int addr, unsigned long int len);
    void setFlashFont(unsigned long int addr);
    void setFileFont(const char *fname);
    void runCommandSet(unsigned long int addr);
    size_t read(void);

    //--- new function on V3.3 firmware ----//
    void writeE2prom(unsigned int addr, unsigned int len, unsigned char *data);
    void readE2prom(unsigned int addr, unsigned int len);

    //--- new function on V3.0 firmware ----//

    void setBgColor(uint8_t color); //set current color as background
    void setDrawWindow(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
    void resetDrawWindow(void);
    void cleanDrawWindow(void);
    //---end of V3.0 functions

    void displayStartScreen(uint8_t m);
    void setMode(uint8_t m); //set display mode

    void setTextPosBack(void); //set text position back to previous, only one back allowed

    void setTextPosOffset(char xoffset, char yoffset);
    void setLinePattern(uint8_t pattern);
    void setLCDChip(uint8_t chip); //only for universal LCD adapter

    void setBackLight(uint8_t bl);
    void digitalOutput(uint8_t x);
    void flushScreen(uint8_t bl);
    void downloadStartScreen(uint16_t lon, const unsigned char *data);
    void downloadUserFont(int lon, const unsigned char *data, uint8_t sect);
    void flashWrite(unsigned long int addr, unsigned long int len, const unsigned char *data);
    void flashWrite(unsigned long int addr, unsigned long int len, unsigned char *data);
    void manualCommand(unsigned char c);
    void manualData(unsigned char d);
    void setSPIMode(unsigned char mode);
    void negativeScreen(char f);
    void showVideo(int x, int y, unsigned char w, unsigned char h,unsigned char f);
    
    //V7 mini file system
    void formatFlash(void);     //format flash memory
    void setFlashMode(char m);  //set the flash memory operating mode
    void reserveFlash(uint32_t length); //reserve flash memory space for address access
    void JPEG(const char *filename);    //display .jpg image file in flash memroy
    void readFile(const char *fname, uint32_t start,uint32_t length);  //read file
    char saveFile(const char *fname, const uint8_t *data);   //save data to a file
    void delFile(const char *fname);    //delete a file
    void renameFile(const char *oldname,const char *newname);   //rename a file
    void runFile(const char *fname);    //run commands set in a file
    void nextJPG(void);     //scan, search and display next .jpg file
    void getDisplay(void);  //get display panel info
    
private:
    unsigned long _Baud;
    HardwareSerial *_mySerial;
    uint8_t _I2Caddress;
    TwoWire *_myWire;
    uint8_t _Clockpin;
    uint8_t _Datapin;
    uint8_t _SSpin;
    uint8_t _SIpin;
    uint8_t _Comdelay;
};

#endif
