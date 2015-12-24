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
void BL_EHInit()
{
    m_logFile = fopen(EH_FILENAME, "w");

    if(m_logFile)
        m_ehInit = 1;
    else
        m_logFile=stdout; // default back to standard output
}

// logs the message
void BL_EHLog(const char* str)
{
    fprintf(m_logFile, str);
}

// flushes to output
void BL_EHFlush()
{
    if(m_ehInit)
        fflush(m_logFile);
}

// Clean up
void BL_EHQuit()
{
    if(m_ehInit) fclose(m_logFile);
    m_ehInit = 0;
}
