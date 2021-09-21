#include <Arduino.h>

extern void initMcp(unsigned char address, unsigned int iodir, unsigned int pullUp) ;
extern void mcpWrite(unsigned char pin, unsigned char state) ;
extern unsigned char mcpRead(unsigned char pin) ;