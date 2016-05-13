//////////////////////////////////////////
// ErrorHandler.c
// Implements function to log
// in game errors
//////////////////////////////////////////
#include "ErrorHandler.h"

#define EH_FILENAME "status.log"

static int m_ehInit = 0;
static FILE* m_logFile = NULL;

// Initialises the error logger
void EHInit()
{
    m_logFile = fopen(EH_FILENAME, "w");

    if(m_logFile)
        m_ehInit = 1;
    else
        m_logFile=stdout; // default back to standard output
}

// logs the message
void EHLog(const char* str)
{
    fprintf(m_logFile, str);
}

// flushes to output
void EHFlush()
{
    if(m_ehInit)
        fflush(m_logFile);
}

// Clean up
void EHQuit()
{
    if(m_ehInit) fclose(m_logFile);
    m_ehInit = 0;
}
