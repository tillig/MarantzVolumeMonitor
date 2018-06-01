#ifndef _BUTTONMANAGER_h
#define _BUTTONMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif

#include "DFRobotLCDShield.h"

class ButtonManagerClass
{
 public:
    int readButton();
    int waitForButtonPress();
};

extern ButtonManagerClass ButtonManager;

#endif

