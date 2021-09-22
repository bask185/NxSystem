#include "src/XpressNetMaster.h"
#include "src/eeprom.h"
#include "src/makeNumber.h"
#include "src/debounceClass.h"
#include "src/macros.h"
#include "src/stateMachineClass.h"
#include <Wire.h>

const int nInputModules = 4 ; // make me variable in setup.
const int nInputs = 64 ;
#define portA	0x12
#define portB	0x13
#define iodirRegA	0x00
#define pullUpRegA	0x0C
#define mcpAddress  0x20

const int pointsPerStreet = 16 ;


enum buttonStates
{
    getFirstBtn ,
    getSecondBtn ,
    settingPoints ,
    teachInStreets ,
} btnState ;

Debounce inputs[nInputs] ;
Debounce controlButton ;

/* XpressNet */
XpressNetMasterClass XpressNet ;

uint8_t firstButton  = 0xFF ;
uint8_t secondButton = 0xFF ;
uint8_t lastPressedButton = 0xFF ;
uint8_t prevPressedButton = 0xFF ;
uint8_t buttonPressed = false ;
uint8_t controlState ;
uint8_t lastPressedInput ;
uint8_t returnCode ;
uint16_t street[ pointsPerStreet ] ;

void debounceInputs()
{
    REPEAT_MS( 20 ) ;
    
    controlButton.debounce() ;
    
    for( int module = 0 ; module < nInputModules ; module ++ )
    {
        Wire.beginTransmission( mcpAddress + module ) ;
        Wire.write( portA ) ;
        Wire.endTransmission() ;
        Wire.requestFrom( mcpAddress + module, 2 ) ;             // get port A and B
        uint16_t input = (Wire.read() << 8) | Wire.read() ;
        
        for( int pin = 0 ; pin < 16 ; pin += 2 )
        {
            bool state = input >> pin ;
            inputs[ nInputModules * 16 + pin ].debounce( state ) ; // deboune all 64 input pins
        }
    }
    
    END_REPEAT
}

void readButtons()
{    
    for( int pin = 0 ; pin < nInputs ; pin ++ )
    {
        uint8_t state = inputs[pin].getState() ; // checks if any of the debounced I2C inputs has changed state

        if( state == FALLING )
        {
            buttonPressed = true ;
            lastPressedButton = pin ; // important for teaching in points
        }
        
        if( firstButton == 0xFF )
        {
            if( state == FALLING )
            {
                firstButton = pin ;
            }
        }
        else
        {
            if( state == FALLING )
            {
                secondButton = pin ;
            }
            if( state == RISING )
            {
                secondButton = 0xFF ;   // if a or both buttons are released, reset these
                firstButton  = 0xFF ;
            }
        }
    }
}


bool setStreets()
{
    static uint8_t index = 0 ;

    REPEAT_MS( pointInterval ) ;
    if( street[index] == 0xFFFF )                       // if address == 0xFFFF, we have set the last point
    {
        quit ;
        index = 0 ;
        return true ;
    }
    else
    {
        if( ++ index == pointsPerStreet ) goto quit ;   // if we have set 16 points (max) we are also done.

        uint16_t address =  street[index] & 0x03FF ;    
        uint8_t  state   =  street[index] >> 15 ;
        XpressNet.SetTrntPos( address, state, 1  ) ;       // set the point
        // delay(20);
        // XpressNet.SetTrntPos( address, state, 0  ) ;     // this may be needed due to buggy library "feature"
    }
    END_REPEAT

    return false ;
}


/* XPRESSNET EVENT */
void notifyXNetTrnt(uint16_t Address, uint8_t data)
{
    if( sm.getState() == teachinPoints )            // if we are teaching points we need to store incomming address and state in EEPROM
    {
        // storePoint( lastPressedButton, Address | (data << 15) ) ;
    }

    if( sm.getState() == readButtons )              // if we are in Idle mode, only update LED's, should work for occupance detectors as well as points
    {
        // uint8_t IO = getIO( Address ) ;          // fetch which IO belongs to this point from EEPROM
        // mcpWrite( IO + 1, state ) ;                  // than update the LED (the IO behind in the input)
    }
}

/* INPUT BUTTON IS PRESSED */
void inputEvent( uint8_t pinNumber )
{
    
}



/* CONTROL BUTTON IS PRESSED */
void controlButtonEvent()
{
    
}


void setup()
{
    sm.nextState( readButtons, 0 ) ;
    controlButton.begin( 3 ) ; // attach pin number to the control button
    for( int i = 0 ; i <= nInputs ; i ++ ) inputs[i].begin( -1 ) ; // flag that external input is used opposed to digitalRead() 
}

void loop()
{
    debounceInputs() ;
    readButtons() ;
    
    XpressNet.update() ;
}


/*
void updateLED( uint8_t number )
void coupleDCCtoNumber( uint16_t dccAddress ,uint8_t lastPressedNumber)
void addDetectorToStreet( uint16_t detector )
void addPointToStreet()
void setNewStreet( uint8_t firstNumber, uint8_t secondNumber )
void setStreet()
void updateMode() 



*/