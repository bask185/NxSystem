// HEADER FILES
#include <Arduino.h>
#include "modeHandling.h"
#include "src/basics/macros.h"
#include "src/basics/io.h"
#include "src/basics/stateMachineClass.h"

StateMachine sm ;

#define beginState modeHandlingIDLE

// VARIABLES

// FUNCTIONS
extern void modeHandlingInit(void)
{
    sm.nextState( beginState, 0 ) ;
}

// STATE FUNCTIONS
StateFunction( readButtons )
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

StateFunction( layStreet )
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

StateFunction( teachinPoints )
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
