#include "eeprom.h"  
#include <EEPROM.h> // arduino library

/* note. There are 32 buttons possible

uint32_t allocateAddress( uint8_t firstButton, uint8_t secondButton )
{
    
}
*/

const int accessoryOffset  = 0x0000;
const int maxPoints        = 0x0040; // 64D

void storePoint( uint8_t pressedButton, uint16_t Address )
{
    uint16_t eeAddress = pressedButton * 2 + accessoryOffset ;       // accesories are stored between 0x0000 - 0x007F
    EEPROM.put( eeAddress, Address ) ;
}

void addDetectorToStreet( uint16_t address )
{
}


uint8_t getIO( uint16_t matchAddress )           // fetch which IO belongs to this point from EEPROM
{
    for( int i = 0 ; i < maxPoints ; i ++ )
    {
        uint16_t pointAddress ;
        uint16_t eeAddress = i * 2 ; // 0 - 2 - 4 etc

        EEPROM.get( eeAddress, pointAddress ) ; // should be I2C
        pointAddress &= 0x03FF ;
        
        if( pointAddress == matchAddress ) return i * 2 ; // there are 2 IO used for every accesory, so multiply i by 2 to return the actual IO pin linked to the point address
    }
    
    return 0xFF ;
}

uint16_t getDccAddress( uint8_t pinNumber )
{
    uint16_t dccAddress ;

    EEPROM.get( pinNumber * 2, dccAddress  ) & 0x7FFF ;

    return dccAddress ;
}

// mem links	    0x1080 - 0x127F  (0x0200 bytes)
void getPoints( uint16_t *street, uint8_t firstButton, uint8_t secondButton )        // spoons EEPROM memory into this array 'street'
{
    
}