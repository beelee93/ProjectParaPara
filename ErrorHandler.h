//////////////////////////////////////////
// ErrorHandler.h
// Defines function prototypes to log
// in game errors
//////////////////////////////////////////

#ifndef ERRORHANDLER_H_INCLUDED
#define ERRORHANDLER_H_INCLUDED
#include <stdio.h>

// Initialises the error logger
void BL_EHInit();

// Logs the message
void BL_EHLog(const char* str);

// Flushes to output
void BL_EHFlush();

// Clean up
void BL_EHQuit();


#endif // ERRORHANDLER_H_INCLUDED
