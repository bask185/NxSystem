#include <Arduino.h>

#ifndef _eeprom
#define _eeprom

extern uint32_t allocateAddress( uint8_t, uint8_t) ;
extern uint8_t getIO( uint16_t ) ;
extern void storePoint( uint8_t pressedButton, uint16_t Address ) ;
extern void addDetectorToStreet( uint16_t ) ;

const int detectorAddresses = 800 ;
uint16_t getDccAddress( uint8_t ) ;

#endif


