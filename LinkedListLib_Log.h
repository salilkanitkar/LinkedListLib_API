#ifndef HEADER_LINKEDLISTLIB_LOG_H // include guard
#define HEADER_LINKEDLISTLIB_LOG_H

#include "LinkedListLib.h"
#include "LinkedListLib_int.h"

extern char *theLogFileName;
extern int   theLogFileExists;


/******************************************************************************
 * enum LinkedListLib_LogLevel --
 *    default is DEBUG (not INFO)
 *    TODO: Provide a way to change logLevel
 *****************************************************************************/

typedef enum LinkedListLib_LogLevel {
   LOG_LEVEL_INFO,
   LOG_LEVEL_ERROR,
   LOG_LEVEL_DEBUG,
   LOG_LEVEL_TRACE
}LinkedListLib_LogLevel;


/******************************************************************************
 * Function LinkedListLib_LogLevel_To_Str --
 *****************************************************************************/

char * LinkedListLib_LogLevel_To_Str(LinkedListLib_LogLevel logLevel);


/******************************************************************************
 * Function LogMessage --
 *    This function does the work of actually logging a message
 *****************************************************************************/

int LogMessage(const char *funcName,
               LinkedListLib_LogLevel logLevel,
               char *format,
               ...);


/******************************************************************************
 * Helper Functions for LogMessage --
 *****************************************************************************/

int AppendPrefixToLogMessage(char *logMessage,
                             const char *funcName,
                             LinkedListLib_LogLevel logLevel);

int WriteLogMessageToFile(char *logMessage);


/******************************************************************************
 * Macros for Logging --
 *    The LinkedListLib should be using only these macros
 *****************************************************************************/

#define LINKEDLISTLIB_TOKEN       "LinkedListLib"
#define LOG_MESSAGE_MAX_LEN       1024
#define LOG_FILE_NAME_MAX_LEN     1024
#define LOG_LEVEL_DEFAULT         LOG_LEVEL_DEBUG
#define LOG_FUNC_NAME             __FUNCTION__

#define LOG(logLevel, ...)                               \
   do {                                                  \
      LogMessage(LOG_FUNC_NAME, logLevel, __VA_ARGS__);  \
   } while (0)

#define LOG_INFO(...)   LOG(LOG_LEVEL_INFO,  __VA_ARGS__)
#define LOG_ERROR(...)  LOG(LOG_LEVEL_ERROR, __VA_ARGS__)
#define LOG_DEBUG(...)  LOG(LOG_LEVEL_DEBUG, __VA_ARGS__)
#define LOG_TRACE(...)  LOG(LOG_LEVEL_TRACE, __VA_ARGS__)

// TODO: Errors encountered while initializing Logging are displayed via printf
//       Must use DebugOutputStrinA() for that.

#endif // include guard
