//////////////////////////////////////////
// ErrorHandler.h
// Defines function prototypes to log
// in game errors
//////////////////////////////////////////

#ifndef ERRORHANDLER_H_INCLUDED
#define ERRORHANDLER_H_INCLUDED
#include <stdio.h>

// Initialises the error logger
void EHInit();

// Logs the message
void EHLog(const char* str);

// Flushes to output
void EHFlush();

// Clean up
void EHQuit();


#endif // ERRORHANDLER_H_INCLUDED
