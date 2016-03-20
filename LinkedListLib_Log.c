#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <windows.h>

#include "LinkedListLib.h"
#include "LinkedListLib_int.h"
#include "LinkedListLib_Log.h"

char *theLogFileName   = NULL;
int   theLogFileExists = FALSE;


/******************************************************************************
 * Function LinkedListLib_LogLevel_To_Str --
 *****************************************************************************/

char* LinkedListLib_LogLevel_To_Str(LinkedListLib_LogLevel logLevel) // IN
{
   switch (logLevel) {
      case LOG_LEVEL_INFO:
         return "LOG_LEVEL_INFO";

      case LOG_LEVEL_ERROR:
         return "LOG_LEVEL_INFO";

      case LOG_LEVEL_DEBUG:
         return "LOG_LEVEL_DEBUG";

      case LOG_LEVEL_TRACE:
         return "LOG_LEVEL_TRACE";

       default:
          return "Unknown";
   }
}


/******************************************************************************
 * Function LogMessage --
 *    This function does the work of actually logging a message
 *****************************************************************************/

int LogMessage(const char *funcName,            // IN
               LinkedListLib_LogLevel logLevel, // IN
               char *format,                    // IN
               ...)                             // IN
{
   char str[LOG_MESSAGE_MAX_LEN];

   if (!MainLinkedListLibInstExists()) {
      return FALSE;
   }

   if (logLevel > theLinkedListLibInst->m_logLevel) {
      // Do not log the message if it is of higher logLevel
      return TRUE;
   }

   // Add a prefix: funcName and a letter depending on the logLevel
   int logPrefixLen = AppendPrefixToLogMessage(str, funcName, logLevel);

   va_list args;
   va_start(args, format);
   vsprintf(str + logPrefixLen, format, args);
   va_end(args);

   if (!WriteLogMessageToFile(str)) {
      printf("%s(): Failed to write Log message to LogFile! \n", __FUNCTION__);
      return FALSE;
   }

   return TRUE;
}


/******************************************************************************
 * Function AppendPrefixToLogMessage --
 *    Appends the Log message prefix to the Log message
 *****************************************************************************/

int AppendPrefixToLogMessage(char *logMessage,                // IN
                             const char *funcName,            // IN
                             LinkedListLib_LogLevel logLevel) // IN
{
   char logLevel_letter;

   if (!logMessage) {
      printf("%s(): logMessage is NULL .... \n", __FUNCTION__);
      return 0;
   }

   switch (logLevel) {
      case LOG_LEVEL_INFO:
         logLevel_letter = 'I';
         break;

      case LOG_LEVEL_ERROR:
         logLevel_letter = 'E';
         break;

      case LOG_LEVEL_DEBUG:
         logLevel_letter = 'D';
         break;

      case LOG_LEVEL_TRACE:
         logLevel_letter = 'T';
         break;

      default:
         logLevel_letter = 'D';
         break;
   }

   int numChars = sprintf(logMessage, "%s: %c : %s(): ", LINKEDLISTLIB_TOKEN,
                                                         logLevel_letter,
                                                         funcName);
   if (numChars == -1) {
      printf("%s(): sprintf error ! \n", __FUNCTION__);
      return 0;
   }

   return numChars;
}


/******************************************************************************
 * Function WriteLogMessageToFile --
 *    Appends the Log message to the Log file
 *****************************************************************************/

int WriteLogMessageToFile(char *logMessage) // IN
{
   unsigned int pid = 0;

   if (!theLogFileName) {
      theLogFileName = (char *)malloc(sizeof(*theLogFileName) * LOG_FILE_NAME_MAX_LEN);

      // Generate logFileName
      pid = GetCurrentProcessId();
      sprintf(theLogFileName, "%s-%d.log", LINKEDLISTLIB_TOKEN, pid);
   }

   FILE *logFp = NULL;

   if (theLogFileExists) {
      logFp = fopen(theLogFileName, "a");
   } else {
      logFp = fopen(theLogFileName, "w");
      theLogFileExists = TRUE;
   }

   if (!logFp) {
      printf("%s(): Failed to open file %s for writing logMessage .. \n", __FUNCTION__, theLogFileName);
      return FALSE;
   }

   // Write the message to the log file
   fprintf(logFp, "%s\n", logMessage);
   fflush(logFp);
   fclose(logFp);

   return TRUE;
}
