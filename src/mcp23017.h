#include <Arduino.h>

#define portA	0x12
#define portB	0x13
#define iodirRegA	0x00
#define pullUpRegA	0x0C
#define mcpAddress  0x20

extern void initMcp(unsigned char address, unsigned int iodir, unsigned int pullUp) ;
extern void mcpWrite(unsigned char pin, unsigned char state) ;
extern unsigned char mcpRead(unsigned char pin) ;