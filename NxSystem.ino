#include "src/XpressNetMaster.h"
#include "src/eeprom.h"
#include "src/makeNumber.h"
#include "src/debounceClass.h"
#include "src/macros.h"
#include <Wire.h>

const int nInputModules = 4 ; // make me variable in setup.
const int nInputs = 64 ;
#define portA	0x12
#define portB	0x13
#define iodirRegA	0x00
#define pullUpRegA	0x0C
#define mcpAddress  0x20

enum modes
{
    IDLE ,
    TEACH_IN_STREETS ,
    TEACH_IN_ACCESSORIES ,
} mode ;

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
uint8_t controlState ;
uint8_t lastPressedInput ;

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
        
        for( int pin = 0 ; pin < 16 ; pin ++ )
        {
            bool state = input >> pin ;
            inputs[ nInputModules * 16 + pin ].debounce( state ) ; // deboune all 64 input pins
        }
    }
    
    END_REPEAT
}


void control()
{
    static bool discardRising ;
    static uint32_t prevTime = 0;
    uint32_t currentTime = millis() ;
    controlState = controlButton.getState() ;
    
    if( controlState == FALLING )
    {
        prevTime = currentTime ;
        discardRising = false ;
    }
    
    if( controlState == RISING && discardRising == false )    // if button is released before 2 seconds...
    {
        if( mode == IDLE ) mode = TEACH_IN_STREETS ;
        else               mode = IDLE ;
    }
    
    if( controlState == LOW && currentTime - prevTime >= 2000 ) // if button is pressed for 2 seconds....
    {
        discardRising = true ;                                  // discard rising flank of button
        
        if( mode == IDLE ) mode = TEACH_IN_ACCESSORIES ;
    }
}

void teachInAccessories()
{
}

void readButtons()
{    
    for( int pin = 0 ; pin < nInputs ; pin ++ )
    {
        uint8_t state = inputs[pin].getState() ;
        // if( state == FALLING || state == RISING ) 
        // {
        //     clear() ;
        //     print(F("BUTTON: ")) ;
        //     printNumberU(pin,1);
        //     if( state == FALLING ) printNumber(0,1);
        //     if( state ==  RISING ) printNumber(1,1);
        // }
        if( state == FALLING )
        {
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


void setStreets()
{
    if( pointsAreSet == false )
    {
        
    }
}


StateMachine sm ;

// STATE FUNCTIONS
StateFunction( readButtons )
{
    if( sm.entryState() )
    {
        
    }
    if( sm.onState() )
    {
        if( controlState == FALLING 
        || (firstButton != 0xFF && secondButton != 0xFF) ) sm.exit() ;
    }
    if( sm.exitState() )
    {
        // calculate eeadress here in case we need to set points
    }
    return sm.endState() ;
}

StateFunction( layStreet )
{
    if( sm.entryState() )
    {
        // calculate eeprom address and initialize setting street. 
    }
    if( sm.onState() )
    {
        if( setStreets() ) sm.exit() ; // set streets and exit when done
    }
    if( sm.exitState() )
    {
        
    }
    return sm.endState() ;
}

StateFunction( teachinPoints )
{
    if( sm.entryState() )
    {
        
    }
    if( sm.onState() )                              // the xpressnet notify funcion now stores information
    {
        if( controlState == FALLING ) sm.exit() ;
    }
    if( sm.exitState() )
    {
        
    }
    return sm.endState() ;
}

StateFunction( teachinStreets )
{
    if( sm.entryState() )
    {
        
    }
    if( sm.onState() )
    {
        
        sm.exit() ;
    }
    if( sm.exitState() )
    {
        
    }
    return sm.endState() ;
}


// STATE MACHINE
extern uint8_t modeHandling()
{
    STATE_MACHINE_BEGIN

    State(readButtons) {
        sm.nextState( teachinPoints, 0 ) ;
        sm.nextState( layStreet, 0 ) ;
        sm.nextState( teachinStreets, 0 ) ; }

    State(layStreet) {
        sm.nextState( readButtons, 0 ) ; }

    State(teachinPoints) {
        sm.nextState( readButtons, 0 ) ; }

    State(teachinStreets) {
        sm.nextState( readButtons, 0 ) ; }

    STATE_MACHINE_END
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
    modeHandling() ;
    
    XpressNet.update() ;
}