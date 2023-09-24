//Digole Digital Solutions: www.digole.com
#include "DigoleSerial.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

/*
// Communication set up command
 * "SB":Baud (ascII bytes end with 0x00/0x0A/0x0D) -- set UART Baud Rate
 * "SI2CA":Address(1 byte <127) -- Set I2C address, default address is:0x27
 * "DC":1/0(1byte) -- set config display on/off, if set to 1, displayer will display current commucation setting when power on
// Text Function command
 * "CL": -- Clear screen--OK
 * "CS":1/0 (1 byte)-- Cursor on/off
 * "TP":x(1 byte) y(1 byte) -- set text position
 * "TT":string(bytes) end with 0x00/0x0A/0x0D -- display string under regular mode
// Graphic function command
 * "GP":x(1byte) y(1byte) -- set current graphic position
 * "DM":"C/!/~/&/|/^"(ASCII 1byte) -- set drawing mode--C="Copy",! and ~ = "Not", & = "And", | = "Or", ^ = "Xor"
 * "SC":1/0 (1byte) -- set draw color--only 1 and 0
 * "LN":x0(1byte) y0(1byte) x1(1byte) y2(1byte)--draw line from x0,y0 to x1,y1,set new pot to x1,y1
 * "LT":x(1byte) y(1byte) -- draw line from current pos to x,y
 * "CC":x(1byte) y(1byte) ratio(byte) -- draw circle at x,y with ratio
 * "DP":x(1byte) y(1byte) Color(1byte) -- draw a pixel--OK
 * "DR":x0(1byte) y0(1byte) x1(1byte) y2(1byte)--draw rectangle, top-left:x0,y0; right-bottom:x1,y1
 * "FR":x0(1byte) y0(1byte) x1(1byte) y2(1byte)--draw filled rectangle, top-left:x0,y0; right-bottom:x1,y1
 */

// that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

int DigoleSerialDisp::readInt(void) {
    int a;
    a = read1() << 8;
    a += read1();
    return a;
}

void DigoleSerialDisp::preprint(void) {
    //write((uint8_t)0);
    writeStr("TT");
}

void DigoleSerialDisp::write24b(uint32_t d) //write 3 bytes format integer to serial port
{
    write(d >> 16);
    write(d >> 8);
    write(d);
}

/*----------Functions for Graphic LCD/OLED adapters only---------*/
void DigoleSerialDisp::drawBitmap(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap) {
    uint8_t i = 0;
    if ((w & 7) != 0)
        i = 1;
    writeStr("DIM");
    write2B(x); //x;
    write2B(y);
    write2B(w);
    write2B(h);
    for (unsigned int j = 0; j < h * ((w >> 3) + i); j++) {
        write(pgm_read_byte_near(bitmap + j));
        //        delay(5);
    }
}

void DigoleSerialDisp::setRot90(void) {
    writeStr("SD1");
}

void DigoleSerialDisp::setRot180(void) {
    writeStr("SD2");
}

void DigoleSerialDisp::setRot270(void) {
    writeStr("SD3");
}

void DigoleSerialDisp::undoRotation(void) {
    writeStr("SD0");
}

void DigoleSerialDisp::setRotation(uint8_t d) {
    writeStr("SD");
    write2B(d);
}

void DigoleSerialDisp::setContrast(uint8_t c) {
    writeStr("CT");
    write2B(c);
}

void DigoleSerialDisp::drawBox(unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
    writeStr("FR");
    write2B(x);
    write2B(y);
    write2B(x + w);
    write2B(y + h);
}

void DigoleSerialDisp::drawCircle(unsigned int x, unsigned int y, unsigned int r, uint8_t f) {
    writeStr("CC");
    write2B(x);
    write2B(y);
    write2B(r);
    write2B(f);
}

void DigoleSerialDisp::drawDisc(unsigned int x, unsigned int y, unsigned int r) {
    drawCircle(x, y, r, 1);
}

void DigoleSerialDisp::drawFrame(unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
    writeStr("DR");
    write2B(x);
    write2B(y);
    write2B(x + w);
    write2B(y + h);
}

void DigoleSerialDisp::drawPixel(unsigned int x, unsigned int y) {
    writeStr("DP");
    write2B(x);
    write2B(y);
}

void DigoleSerialDisp::drawPixel(unsigned int x, unsigned int y, uint8_t color) {
    writeStr("SC");
    write(color);
    writeStr("DP");
    write2B(x);
    write2B(y);
}

void DigoleSerialDisp::drawLine(unsigned int x, unsigned int y, unsigned int x1, unsigned int y1) {
    //    writeStr("\x1B\x10");
    writeStr("LN");
    write2B(x);
    write2B(y);
    write2B(x1);
    write2B(y1);
}

void DigoleSerialDisp::drawLineTo(unsigned int x, unsigned int y) {
    writeStr("LT");
    write2B(x);
    write2B(y);
}

void DigoleSerialDisp::drawHLine(unsigned int x, unsigned int y, unsigned int w) {
    drawLine(x, y, x + w, y);
}

void DigoleSerialDisp::drawVLine(unsigned int x, unsigned int y, unsigned int h) {
    drawLine(x, y, x, y + h);
}

void DigoleSerialDisp::nextTextLine(void) {
    write((uint8_t) 0);
    writeStr("TRT");
}

void DigoleSerialDisp::setFont(uint8_t font) {
    writeStr("SF");
    write(font);
}

void DigoleSerialDisp::setColor(uint8_t color) {
    writeStr("SC");
    write(color);
}

void DigoleSerialDisp::directCommand(uint8_t d) {
    writeStr("MCD");
    write(d);
}

void DigoleSerialDisp::directData(uint8_t d) {
    writeStr("MDT");
    write(d);
}

void DigoleSerialDisp::moveArea(unsigned int x0, unsigned int y0, unsigned int w, unsigned int h, char xoffset, char yoffset) {
    writeStr("MA");
    write2B(x0);
    write2B(y0);
    write2B(w);
    write2B(h);
    write(xoffset);
    write(yoffset);
}

void DigoleSerialDisp::drawColorImg(unsigned char format, unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap) {
    writeStr("EDIM");
    write(format);
    write2B(x); //x;
    write2B(y);
    write2B(w);
    write2B(h);
    format = format - 48;
    for (unsigned long int j = 0; j < (h * w) * format; j++) {
        write(pgm_read_byte(bitmap + j));
        //        delay(5);
    }
}

void DigoleSerialDisp::drawBitmap256(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap) { //display 256 color image
    //    uint8_t i = 0;
    writeStr("EDIM1");
    write2B(x); //x;
    write2B(y);
    write2B(w);
    write2B(h);
    for (unsigned long int j = 0; j < h * w; j++) {
        write(pgm_read_byte(bitmap + j));
        //        delay(5);
    }
}

void DigoleSerialDisp::drawBitmap65K(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap) { //display 256 color image
    //    uint8_t i = 0;
    writeStr("EDIM2");
    write2B(x); //x;
    write2B(y);
    write2B(w);
    write2B(h);
    for (unsigned long int j = 0; j < h * w * 2; j++) {
        write(pgm_read_byte(bitmap + j));
    }
}

void DigoleSerialDisp::drawBitmap262K(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const uint8_t *bitmap) { //display 256 color image
    //    uint8_t i = 0;
    writeStr("EDIM3");
    write2B(x); //x;
    write2B(y);
    write2B(w);
    write2B(h);
    for (unsigned long int j = 0; (j < h * w * 3); j++) {
        write(pgm_read_byte(bitmap + j));
    }
}

void DigoleSerialDisp::setTrueColor(uint8_t r, uint8_t g, uint8_t b) { //Set true color
    //    uint8_t i = 0;
    writeStr("ESC");
    write(r);
    write(g);
    write(b);
}

void DigoleSerialDisp::drawStr(unsigned int x, unsigned int y, const char *s) {
    writeStr("TP");
    write2B(x);
    write2B(y);
    writeStr("TT");
    writeStr(s);
    write((uint8_t) 0);
}

void DigoleSerialDisp::setPrintPos(unsigned int x, unsigned int y, uint8_t graph) {
    if (graph == 0) {
        writeStr("TP");
        write2B(x);
        write2B(y);
    } else {
        writeStr("GP");
        write2B(x);
        write2B(y);
    }
}

void DigoleSerialDisp::moveTo(int x, int y) {
    writeStr("GP");
    write2B(x);
    write2B(y);
}

void DigoleSerialDisp::setLCDColRow(uint8_t col, uint8_t row) {
    writeStr("STCR");
    write(col);
    write(row);
    writeStr("\x80\xC0\x94\xD4");
}

void DigoleSerialDisp::setTextPosAbs(unsigned int x, unsigned int y) {
    writeStr("ETP");
    write2B(x);
    write2B(y);
}

//----Touch screen functions-----//

void DigoleSerialDisp::calibrateTouchScreen(void) {
    writeStr("TUCHC");
}

void DigoleSerialDisp::readTouchScreen() {
    cleanBuffer();
    writeStr("RPNXYW");
}

void DigoleSerialDisp::readTouchScreen(int *x, int *y) {
    cleanBuffer();
    readTouchScreen();
    *x = readInt();
    *y = readInt();
}

void DigoleSerialDisp::readTouchInst(void) {
    cleanBuffer();
    writeStr("RPNXYI");
}

void DigoleSerialDisp::readTouchPres(void) {
    cleanBuffer();
    writeStr("RPNXYP");
}

void DigoleSerialDisp::readClick(void) //read a click on touch screen
{
    cleanBuffer();
    writeStr("RPNXYC");
}

void DigoleSerialDisp::readClick(int *x, int *y) //read a click on touch screen
{
    cleanBuffer();
    readClick();
    *x = readInt();
    *y = readInt();
}

int DigoleSerialDisp::readBattery(void) {
    int c;
    cleanBuffer();
    writeStr("RDBAT");
    delay(50);
    c = read1();
    c <<= 8;
    c |= read1();
    return c;
}

int DigoleSerialDisp::readAux(void) {
    int c;
    cleanBuffer();
    writeStr("RDAUX");
    //    delay(50);
    c = read1();
    c <<= 8;
    c += read1();
    return c;
}

int DigoleSerialDisp::readTemperature(void) {
    int c;
    cleanBuffer();
    writeStr("RDTMP");
    delay(50);
    c = read1();
    c <<= 8;
    c |= read1();
    //    c=readInt();
    return c;
}
//-----Flash memory functions----//

void DigoleSerialDisp::flashErase(unsigned long int addr, unsigned long int length) {
    writeStr("FLMER");
    write(addr >> 16);
    write(addr >> 8);
    write(addr);
    write(length >> 16);
    write(length >> 8);
    write(length);
}

void DigoleSerialDisp::flashReadStart(unsigned long int addr, unsigned long int len) {
    cleanBuffer();
    writeStr("FLMRD");
    write24b(addr);
    write24b(len);
    delay(100);
}

void DigoleSerialDisp::setFlashFont(unsigned long int addr) {
    writeStr("SFF");
    write24b(addr);
}

void DigoleSerialDisp::runCommandSet(unsigned long int addr) {
    writeStr("FLMCS");
    write24b(addr);
}
//V3.3 functions

void DigoleSerialDisp::writeE2prom(unsigned int addr, unsigned int len, unsigned char *data) {
    unsigned char c;
    writeStr("WREP");
    write(addr >> 8);
    write(addr);
    write(len >> 8);
    write(len);
    for (unsigned int i = 0; i < len; i++) {
        c = data[i];
        write(c);
    }
}

void DigoleSerialDisp::readE2prom(unsigned int addr, unsigned int len) {
    cleanBuffer();
    writeStr("RDEP");
    write(addr >> 8);
    write(addr);
    write(len >> 8);
    write(len);
    delay(100);
}

void DigoleSerialDisp::backLightOn(void) {
    writeStr("BL");
    write((uint8_t) 100);
}

void DigoleSerialDisp::backLightOff(void) {
    writeStr("BL");
    write((uint8_t) 0);
}

void DigoleSerialDisp::screenOnOff(uint8_t a) //turn screen on/off
{
    writeStr("SOO");
    write((uint8_t) a);
}

void DigoleSerialDisp::moduleOff(void) //put whole module in sleep: Back light off, screen in sleep, MCU in sleep
{
    writeStr("DNALL");
}

void DigoleSerialDisp::backLightBrightness(uint8_t a) //set backlight brightness,0~100
{
    writeStr("BL");
    write((uint8_t) a);
}

void DigoleSerialDisp::writeStr(const char *s) {
    int i = 0;
    while (s[i] != 0)
        write(s[i++]);
}

void DigoleSerialDisp::disableCursor(void) {
    writeStr("CS0");
}

void DigoleSerialDisp::enableCursor(void) {
    writeStr("CS1");
}

void DigoleSerialDisp::clearScreen(void) {
    writeStr("CL");
}

void DigoleSerialDisp::setI2CAddress(uint8_t add) {
    writeStr("SI2CA");
    write(add);
    _I2Caddress = add;
}

void DigoleSerialDisp::displayConfig(uint8_t v) {
    writeStr("DC");
    write(v);
}

void DigoleSerialDisp::setPushup(char v) {
    writeStr("PUSHT");
    write(v);
}

void DigoleSerialDisp::setTrans(char v) {
    writeStr("TRANS");
    write(v);
}

void DigoleSerialDisp::setTextAlignment(char v) {
    writeStr("ALIGN");
    write(v);
}

void DigoleSerialDisp::setBgColor(uint8_t color) //set current color as background
{

#if Ver>32
    writeStr("BGC");
    write(color);
#else
    writeStr("SC");
    write(color);
    writeStr("BGC");
#endif
}

void DigoleSerialDisp::setDrawWindow(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
    writeStr("DWWIN");
    write2B(x);
    write2B(y);
    write2B(width);
    write2B(height);
}

void DigoleSerialDisp::resetDrawWindow(void) {
    writeStr("RSTDW");
}

void DigoleSerialDisp::cleanDrawWindow(void) {
    writeStr("WINCL");
}
//---end of V3.0 functions

void DigoleSerialDisp::displayStartScreen(uint8_t m) {
    writeStr("DSS");
    write(m);
} //display start screen

void DigoleSerialDisp::setMode(uint8_t m) {
    writeStr("DM");
    write(m);
} //set display mode

void DigoleSerialDisp::setTextPosBack(void) {
    writeStr("ETB");
} //set text position back to previous, only one back allowed

void DigoleSerialDisp::setTextPosOffset(char xoffset, char yoffset) {
    writeStr("ETO");
    write(xoffset);
    write(yoffset);
}

void DigoleSerialDisp::setLinePattern(uint8_t pattern) {
    writeStr("SLP");
    write(pattern);
}

void DigoleSerialDisp::setLCDChip(uint8_t chip) { //only for universal LCD adapter
    writeStr("SLCD");
    write(chip);
}

void DigoleSerialDisp::setBackLight(uint8_t bl) {
    writeStr("BL");
    write(bl);
}

void DigoleSerialDisp::digitalOutput(uint8_t x) {
    writeStr("DOUT");
    write(x);
}

void DigoleSerialDisp::flushScreen(uint8_t bl) {
    writeStr("FS");
    write(bl);
}

void DigoleSerialDisp::downloadStartScreen(uint16_t lon, const unsigned char *data) {
    int j;
    unsigned char b;
    uint8_t c;
    /*    writeStr("FLMER");	//erase flash 
        write((uint8_t)0);
        write((uint8_t)0);
        write((uint8_t)0);
        write((uint8_t)0);
        write(lon >> 8);
        write(lon);
        delay(1000);
     */ writeStr("SSS");
    lon++;
    write((uint8_t) (lon / 256));
    write((uint8_t) (lon % 256));
    delay(2000); //waiting for the display erase the space internal
    b = 0;
    for (j = 0; j < (lon - 1); j++) {
        c = pgm_read_byte_near(data + j);
        write(c);
        if ((++b) == 64) {
            b = 0, delay(_WRITE_DELAY);
        }
    }
    write(255); //indicater of end of it
    delay(_WRITE_DELAY);
}

void DigoleSerialDisp::downloadUserFont(int lon, const unsigned char *data, uint8_t sect) {
    uint8_t c;
    unsigned char b;
    /*----the erase section below for display without flash chip only----*/
    long int adr;
    sect &= 3;
    adr = 2048 + 3584 * sect;
    writeStr("FLMER"); //erase flash, not for V7 and 18K20 firmware
    write24b(adr);
    write((uint8_t) 0);
    write(lon >> 8);
    write(lon);
    delay(1000);
    writeStr("SUF");
    write(sect);
    delay(600); //waiting for the display erase the memory space internal
    /*-----End of erase section -----*/
#if Ver>32
    write((uint8_t) (lon / 256));
    write((uint8_t) (lon % 256));
#else
    write((uint8_t) (lon % 256));
    write((uint8_t) (lon / 256));
#endif
    b = 0;
    for (int j = 0; j < lon; j++) {
        c = pgm_read_byte_near(data + j);
        write(c);
        if ((++b) == 64) {
            b = 0, delay(_WRITE_DELAY);
        }
    }
}

void DigoleSerialDisp::flashWrite(unsigned long int addr, unsigned long int len, const unsigned char *data) {
    unsigned char c, b;
    unsigned long int i;
    writeStr("FLMER"); //erase flash 
    write24b(addr);
    write24b(len);
    delay(1000);
    writeStr("FLMWR");
    write24b(addr);
    write24b(len);
    b = 0;
    for (i = 0; i < len; i++) {
        c = pgm_read_byte(data + i);
        write(c);
        if ((++b) == 64) {
            b = 0, delay(_WRITE_DELAY);
        }
    }
    delay(_WRITE_DELAY);
#ifdef FLASH_CHIP
    //check write memory done
    while (read1() != 17);
#endif
}

void DigoleSerialDisp::flashWrite(unsigned long int addr, unsigned long int len, unsigned char *data) {
    unsigned char c, b;
    unsigned long int i;
    writeStr("FLMER"); //erase flash 
    write24b(addr);
    write24b(len);
    delay(1000);
    writeStr("FLMWR");
    write24b(addr);
    write24b(len);
    b = 0;
    for (i = 0; i < len; i++) {
        c = data[i];
        write(c);
        if ((++b) == 64) {
            b = 0, delay(_WRITE_DELAY);
        }
    }
#ifdef FLASH_CHIP
    //check write memory done
    while (read1() != 17);
#endif
}

void DigoleSerialDisp::manualCommand(unsigned char c) {
    writeStr("MCD");
    write(c);
}

void DigoleSerialDisp::manualData(unsigned char d) {
    writeStr("MDT");
    write(d);
}

void DigoleSerialDisp::setSPIMode(unsigned char mode) {
    if (mode < 4) {
        writeStr("SPIMD");
        write(mode);
    }
}

void DigoleSerialDisp::negativeScreen(char f) {
    writeStr("INV");
    write(f);
}

void DigoleSerialDisp::mcuOff(void) {
    writeStr("DNMCU");
}

void DigoleSerialDisp::cpuOff(void) {
    writeStr("DNMCU");
}

void DigoleSerialDisp::showVideo(int x, int y, unsigned char w, unsigned char h, unsigned char format) {
    writeStr("VIDEO");
    write(x >> 8);
    write(x);
    write(y >> 8);
    write(y);
    write(w);
    write(h);
    write(format);
}

void DigoleSerialDisp::invert(unsigned char onoff) {
    writeStr("INV");
    write(onoff);
}
/****functions for V7 firmware****/
//change flash memory operating mode: 1--file mode, 0--address mode

void DigoleSerialDisp::setFlashMode(char m) {
    writeStr("fMODE");
    write(m);
}

void DigoleSerialDisp::formatFlash(void) {
    setFlashMode(1);
    writeStr("4MAT");
    setFlashMode(0);
}

void DigoleSerialDisp::reserveFlash(uint32_t length) {
    writeStr("FLMRS");
    write(0x55);
    write(0xAA);
    write24b(length);
}

void DigoleSerialDisp::JPEG(const char *filename) {
    writeStr("JPEG");
    writeStr(filename);
    write((char)0);
}

void DigoleSerialDisp::readFile(const char *fname, uint32_t start, uint32_t length) {
    writeStr("fREAD");
    writeStr(fname);
    write((char)0);
    write24b(start);
    write24b(length);
    //read out data....
}

char DigoleSerialDisp::saveFile(const char *fname, const uint8_t *data) {
    if (sizeof (data)) {
        writeStr("fSAVE");
        writeStr(fname);
        if(sizeof(fname)<11)
            write((char)0);
        write24b(sizeof (data));
        for (uint32_t i = 0; i<sizeof (data); i++)
            write(data[i]);
        return read1();
    }
    return 0;
}

void DigoleSerialDisp::delFile(const char *fname) {
    writeStr("fDEL");
    writeStr(fname);
    write((char)0);
}
void DigoleSerialDisp::renameFile(const char *oldname,const char *newname)
{
    writeStr("fREN");
    writeStr(oldname);
    if(sizeof(oldname)<11)
        write((char)0);
    writeStr(newname);
    write((char)0);   
}
void DigoleSerialDisp::runFile(const char *fname)
{
    writeStr("fCS");
    writeStr(fname);
    write((char)0);    
}
void DigoleSerialDisp::setFileFont(const char *fname)
{
    writeStr("SFf");
    writeStr(fname);
    write((char)0);     
}
void DigoleSerialDisp::nextJPG(void)
{
    writeStr("fDPF");
}
void DigoleSerialDisp::getDisplay(void)
{
    writeStr("WHO");    
}