enum modeHandlingStates
{
    modeHandlingIDLE,
    readButtons,
    layStreet,
    teachinPoints,
    teachinStreets
} ;

extern uint8_t modeHandling(void) ; 
extern void modeHandlingInit(void) ; 
