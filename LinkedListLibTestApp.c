#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "LinkedListLibTestApp.h"

#define LOG_MESSAGE_MAX_LEN          1024
#define LOG_MESSAGE_PREFIX_PROGNAME  "LinkedListLibTestApp"

#define LOG_MESSAGE(...)                     \
   do {                                      \
      LogDisplayMessage(__VA_ARGS__);        \
   } while (0)


/******************************************************************************
 *
 * LogDisplayMessage --
 *
 *    Logs a message to stdout
 *
 *****************************************************************************/
void LogDisplayMessage(const char *fmt, ...)
{
   char str[LOG_MESSAGE_MAX_LEN];

   va_list args;
   va_start(args, fmt);
   vsprintf(str, fmt, args);
   va_end(args);

   fprintf(stdout, "%s: %s\n", LOG_MESSAGE_PREFIX_PROGNAME,
                               str);
   return;
}


// Unit Tests functions
int Ut_Init_Uninit_Lists_TestFunc01();
int Ut_Init_Uninit_Lists_TestFunc02();
int Ut_Add_Delete_Elements_TestFunc01();
int Ut_Add_Delete_Elements_TestFunc02();



/******************************************************************************
 *
 * Function main --
 *
 *    Main func of LinkedListLibTestApp
 *
 *****************************************************************************/
int main()
{
   LOG_MESSAGE("%s(): Start LinkedListLibTestApp ....", __FUNCTION__);

   if (!LinkedListLib_Init()) {
      LOG_MESSAGE("%s(): Error in Initializing LinkedListLib", __FUNCTION__);
      goto exit;
   }

   LOG_MESSAGE("%s(): LinkedListLib_Init(): Success", __FUNCTION__);

   ListNode *listHandle;
   if (!LinkedListLib_InitList(&listHandle)) {
      LOG_MESSAGE("%s(): Error in Initializing LinkedList", __FUNCTION__);
      goto exit;
   }

   LOG_MESSAGE("%s(): LinkedListLib_InitList(): Success, listHandle: 0x%p",
                                                            __FUNCTION__, listHandle);

   if (!LinkedListLib_UninitList(listHandle)) {
      LOG_MESSAGE("%s(): Error in Uninitializing LinkedList", __FUNCTION__);
      goto exit;
   }
   listHandle = NULL;

   LOG_MESSAGE("%s(): LinkedListLib_UninitList(): Success", __FUNCTION__);

   // Invoke Ut_Init_Uninit_Lists_TestFunc01()
   if (!Ut_Init_Uninit_Lists_TestFunc01()) {
      LOG_MESSAGE("%s(): Ut_Init_Uninit_Lists_TestFunc01() Failed ...", __FUNCTION__);
   }

   // Invoke Ut_Init_Uninit_Lists_TestFunc02()
   if (!Ut_Init_Uninit_Lists_TestFunc02()) {
      LOG_MESSAGE("%s(): Ut_Init_Uninit_Lists_TestFunc02() Failed ...", __FUNCTION__);
   }

   // Invoke Ut_Add_Delete_Elements_TestFunc01()
   if (!Ut_Add_Delete_Elements_TestFunc01()) {
      LOG_MESSAGE("%s(): Ut_Add_Delete_Elements_TestFunc01() Failed ...", __FUNCTION__);
   }

   // Invoke Ut_Add_Delete_Elements_TestFunc02()
   if (!Ut_Add_Delete_Elements_TestFunc02()) {
      LOG_MESSAGE("%s(): Ut_Add_Delete_Elements_TestFunc02() Failed ...", __FUNCTION__);
   }

   if (!LinkedListLib_Uninit()) {
      LOG_MESSAGE("%s(): Error in Uninitializing LinkedListLib", __FUNCTION__);
      goto exit;
   }

   LOG_MESSAGE("%s(): LinkedListLib_Uninit(): Success", __FUNCTION__);

exit:
   LOG_MESSAGE("%s(): End   LinkedListLibTestApp ....", __FUNCTION__);

   getchar();
   return 0;
}


/******************************************************************************
 *
 * Function Ut_Init_Uninit_Lists_TestFunc01 --
 *
 *    Init LINKEDLISTLIB_MAX_NUM_LISTS LinkedLists - all should succeed
 *    Init one more list - it should fail
 *    Uninit all of LINKEDLISTLIB_MAX_NUM_LISTS - all should succeed
 *
 *****************************************************************************/

int Ut_Init_Uninit_Lists_TestFunc01()
{
   int ok = TRUE;

   ListNode *listHandles[LINKEDLISTLIB_MAX_NUM_LISTS];
   ListNode *listHandleFailure = NULL;
   int i = 0, numLists = 0;

   LOG_MESSAGE("%s(): Starting ... ", __FUNCTION__);

   for (i = 0 ; i < LINKEDLISTLIB_MAX_NUM_LISTS ; i++) {
      if (!LinkedListLib_InitList(&listHandles[i])) {
         LOG_MESSAGE("%s(): Failed to init list #: %d", __FUNCTION__, i);
         ok = FALSE;
         numLists = i;
         goto exit;
      }
#if 0
      LOG_MESSAGE("%s(): LinkedList %d initialized", __FUNCTION__, i);
#endif
      numLists += 1;
   }

#if 0
   LOG_MESSAGE("%s(): numLists: %d", __FUNCTION__, numLists);

   for (i = 0 ; i < numLists ; i++) {
      LOG_MESSAGE("%s(): listHandles[%d]: 0x%p",
                                       __FUNCTION__,
                                       i,
                                       listHandles[i]);
   }
#endif

   if (LinkedListLib_IsFull()) {
      LOG_MESSAGE("%s(): LinkedListLib is full - can not create any more LinkedLists", __FUNCTION__);
   }

   if (LinkedListLib_InitList(&listHandleFailure) == FALSE) {
      LOG_MESSAGE("%s(): LinkedListLib can not Init one more list !", __FUNCTION__);
   }

exit:
   LOG_MESSAGE("%s(): numLists: %d", __FUNCTION__, numLists);


   for (i = 0 ; i < numLists ; i++) {
      if (!LinkedListLib_UninitList(listHandles[i])) {
         LOG_MESSAGE("%s(): Failed to Uninit List #: %d, 0x%p",
                                 __FUNCTION__, i, listHandles[i]);
         ok = FALSE;
      }
#if 0
      LOG_MESSAGE("%s(): LinkedList %d uninitialized", __FUNCTION__, i);
#endif
   }
   numLists = 0;

   LOG_MESSAGE("%s(): Test %s", __FUNCTION__, ok == TRUE ? "Succeeded" : "Failed");
   return ok;
}


/******************************************************************************
 *
 * Function Ut_Init_Uninit_Lists_TestFunc02 --
 *
 *    Init 5 LinkedLists - all should succeed
 *    Uninit 2nd & 4th LinkedList
 *    Init 1 LinkedList - it should take the index "1" - verify this.
 *
 *****************************************************************************/

int Ut_Init_Uninit_Lists_TestFunc02()
{
   int ok = TRUE;

   ListNode *listHandles[LINKEDLISTLIB_MAX_NUM_LISTS];
   int i = 0, testListIndex = 0;
   int numLists = 0, testNumLists = LINKEDLISTLIB_MAX_NUM_LISTS / 2;

   LOG_MESSAGE("%s(): Starting ... ", __FUNCTION__);

   for (i = 0 ; i < testNumLists ; i++) {
      if (!LinkedListLib_InitList(&listHandles[i])) {
         LOG_MESSAGE("%s(): Failed to init list #: %d", __FUNCTION__, i);
         ok = FALSE;
         numLists = i;
         goto exit;
      }
#if 0
      LOG_MESSAGE("%s(): LinkedList %d initialized", __FUNCTION__, i);
#endif
      numLists += 1;
   }

#if 0
   LOG_MESSAGE("%s(): numLists: %d", __FUNCTION__, numLists);

   for (i = 0 ; i < numLists ; i++) {
      LOG_MESSAGE("%s(): listHandles[%d]: 0x%p",
                                       __FUNCTION__,
                                       i,
                                       listHandles[i]);
   }
#endif

   // Uninit 2nd and 4th LinkedList
   if (!LinkedListLib_UninitList(listHandles[1])) {
      LOG_MESSAGE("%s(): Failed to Uninit 2nd LinkedList for handle: 0x%p",
                                 __FUNCTION__, listHandles[1]);
      ok = FALSE;
      goto exit;
   }
   listHandles[1] = NULL;
   numLists -= 1;

   if (!LinkedListLib_UninitList(listHandles[3])) {
      LOG_MESSAGE("%s(): Failed to Uninit 4th LinkedList for handle: 0x%p",
                                 __FUNCTION__, listHandles[3]);
      ok = FALSE;
      goto exit;
   }
   listHandles[3] = NULL;
   numLists -= 1;

   if (!LinkedListLib_InitList(&listHandles[1])) {
      LOG_MESSAGE("%s(): Failed to Init a new LinkedList", __FUNCTION__);
      ok = FALSE;
      goto exit;
   }
   numLists += 1;

   if (!LinkedListLib_ListExists(listHandles[1], &testListIndex)) {
      LOG_MESSAGE("%s(): LinkedListLib_ListExists() Failed !", __FUNCTION__);
      ok = FALSE;
      goto exit;
   }

   LOG_MESSAGE("%s(): The index of newly created LinkedList is: %d",
                           __FUNCTION__, testListIndex);

exit:
   LOG_MESSAGE("%s(): numLists: %d", __FUNCTION__, numLists);


   for (i = 0 ; i <= numLists ; i++) {
      if (listHandles[i] && !LinkedListLib_UninitList(listHandles[i])) {
         LOG_MESSAGE("%s(): Failed to Uninit List #: %d, 0x%p",
                                 __FUNCTION__, i, listHandles[i]);
         ok = FALSE;
      }
#if 0
      LOG_MESSAGE("%s(): LinkedList %d uninitialized", __FUNCTION__, i);
#endif
   }
   numLists = 0;

   LOG_MESSAGE("%s(): Test %s", __FUNCTION__, ok == TRUE ? "Succeeded" : "Failed");
   return ok;
}


/******************************************************************************
 *
 * Function Ut_Add_Delete_Elements_TestFunc01 --
 *
 *    Tests: Add & Delete from rear:->
 *    Init 1 LinkedList
 *    Add LINKEDLISTLIB_MAX_NUM_NODES from rear
 *    Check Number of Elements to be equal to LINKEDLISTLIB_MAX_NUM_NODES
 *    Print LinkedList elements to Log
 *    Add one more element - it should Fail
 *    Delete 1 element from rear
 *    Print LinkedList elements
 *    Add 1 element from rear - Succeed
 *    Print LinkedList elements
 *    Add 1 more element - it should Fail
 *    Delete 2 elements from rear - one by one
 *    Print LinkedList elements
 *    Delete all remaining elements from rear
 *    Print LinkedList elements - this should be empty
 *    Uninit the LinkedList
 *
 *****************************************************************************/

int Ut_Add_Delete_Elements_TestFunc01()
{
   int ok               = FALSE;
   int numElements      = -1;
   int index            = 0;

   int nodeData[LINKEDLISTLIB_MAX_NUM_NODES];

   ListNode *listHandle = NULL;

   LOG_MESSAGE("%s(): Starting ... ", __FUNCTION__);

   if (!LinkedListLib_InitList(&listHandle)) {
      LOG_MESSAGE("%s(): Error in Initializing LinkedList", __FUNCTION__);
      listHandle = NULL;
      goto exit;
   }

   LOG_MESSAGE("%s(): LinkedListLib_InitList(): Success, listHandle: 0x%p",
                                                            __FUNCTION__, listHandle);

   // ---------------------------------------------------------------------------
   // Add LINKEDLISTLIB_MAX_NUM_NODES from rear one-by-one
   // Print Number of elements
   // Print List Elements

   for(index = 0 ; index < LINKEDLISTLIB_MAX_NUM_NODES ; index++) {

      if (!LinkedListLib_AddIntDataNodeFromRear(listHandle, index)) {
         LOG_MESSAGE("%s(): Failed to add element \"%d\" from rear!", index);
         goto exit;
      }

   }

   if (!LinkedListLib_GetListNumElements(listHandle, &numElements)) {
      LOG_MESSAGE("%s(): Failed to get number of elements in the List!");
      goto exit;
   }
   LOG_MESSAGE("%s(): Number of elements in the List: %d",
                                          __FUNCTION__, numElements);


   if (!LinkedListLib_PrintListToLog(listHandle)) {
      LOG_MESSAGE("%s(): Failed to print List elements!", __FUNCTION__);
      goto exit;
   }

   // ---------------------------------------------------------------------------

   // ---------------------------------------------------------------------------
   // Add one more element from rear - it should Fail
   // Delete one element from read
   // Print Number of elements
   // Print List Elements
   // Add one element from rear - this should succeed
   // Print Number of elements
   // Print List Elements
   // Add one more element from rear - this should Fail

   if (LinkedListLib_AddIntDataNodeFromRear(listHandle, index)) {
      LOG_MESSAGE("%s(): This Node addition should have Failed!");
      goto exit;
   }

   if (!LinkedListLib_DeleteIntDataNodeFromRear(listHandle, &nodeData[index-1])) {
      LOG_MESSAGE("%s(): Failed to delete node from rear!", __FUNCTION__);
      goto exit;
   }

   if (!LinkedListLib_GetListNumElements(listHandle, &numElements)) {
      LOG_MESSAGE("%s(): Failed to get number of elements in the List!");
      goto exit;
   }
   LOG_MESSAGE("%s(): Number of elements in the List: %d",
                                          __FUNCTION__, numElements);

   if (!LinkedListLib_PrintListToLog(listHandle)) {
      LOG_MESSAGE("%s(): Failed to print List elements!", __FUNCTION__);
      goto exit;
   }

   if (!LinkedListLib_AddIntDataNodeFromRear(listHandle, index-1)) {
      LOG_MESSAGE("%s(): Failed to add element \"%d\" from rear!", index);
      goto exit;
   }

   if (!LinkedListLib_GetListNumElements(listHandle, &numElements)) {
      LOG_MESSAGE("%s(): Failed to get number of elements in the List!");
      goto exit;
   }
   LOG_MESSAGE("%s(): Number of elements in the List: %d",
                                          __FUNCTION__, numElements);


   if (!LinkedListLib_PrintListToLog(listHandle)) {
      LOG_MESSAGE("%s(): Failed to print List elements!", __FUNCTION__);
      goto exit;
   }

   if (LinkedListLib_AddIntDataNodeFromRear(listHandle, index)) {
      LOG_MESSAGE("%s(): This Node addition should have Failed!");
      goto exit;
   }

   // ---------------------------------------------------------------------------

   // ---------------------------------------------------------------------------
   // Delete two elements from rear, one by one
   // Print Number of elements
   // Print List Elements

   if (!LinkedListLib_DeleteIntDataNodeFromRear(listHandle, &nodeData[index-1])) {
      LOG_MESSAGE("%s(): Failed to delete node from rear!", __FUNCTION__);
      goto exit;
   }

   if (!LinkedListLib_DeleteIntDataNodeFromRear(listHandle, &nodeData[index-2])) {
      LOG_MESSAGE("%s(): Failed to delete node from rear!", __FUNCTION__);
      goto exit;
   }

   if (!LinkedListLib_GetListNumElements(listHandle, &numElements)) {
      LOG_MESSAGE("%s(): Failed to get number of elements in the List!");
      goto exit;
   }
   LOG_MESSAGE("%s(): Number of elements in the List: %d",
                                          __FUNCTION__, numElements);

   if (!LinkedListLib_PrintListToLog(listHandle)) {
      LOG_MESSAGE("%s(): Failed to print List elements!", __FUNCTION__);
      goto exit;
   }

   index -= 2;

   // ---------------------------------------------------------------------------

   while (index > 0) {

      if (!LinkedListLib_DeleteIntDataNodeFromRear(listHandle, &nodeData[index])) {
         LOG_MESSAGE("%s(): Failed to delete node from rear!", __FUNCTION__);
         goto exit;
      }

      index -= 1;
   }

   if (!LinkedListLib_GetListNumElements(listHandle, &numElements)) {
      LOG_MESSAGE("%s(): Failed to get number of elements in the List!");
      goto exit;
   }
   LOG_MESSAGE("%s(): Number of elements in the List: %d",
                                          __FUNCTION__, numElements);

   if (!LinkedListLib_PrintListToLog(listHandle)) {
      LOG_MESSAGE("%s(): Failed to print List elements!", __FUNCTION__);
      goto exit;
   }

   // ---------------------------------------------------------------------------

   ok = TRUE;

exit:
   if (listHandle) {

      if (!LinkedListLib_UninitList(listHandle)) {
         LOG_MESSAGE("%s(): Error in Uninitializing LinkedList", __FUNCTION__);
         ok = FALSE;
      } else {
         LOG_MESSAGE("%s(): LinkedListLib_UninitList(): Success", __FUNCTION__);
      }

      listHandle = NULL;
   }

   LOG_MESSAGE("%s(): Test %s", __FUNCTION__, ok == TRUE ? "Succeeded" : "Failed");
   return ok;
}


/******************************************************************************
 *
 * Function Ut_Add_Delete_Elements_TestFunc02 --
 *
 *    Tests: Add & Delete from front:->
 *    Init 1 LinkedList
 *    Add LINKEDLISTLIB_MAX_NUM_NODES from front
 *    Check Number of Elements to be equal to LINKEDLISTLIB_MAX_NUM_NODES
 *    Print LinkedList elements to Log
 *    Add one more element - it should Fail
 *    Delete 1 element from front
 *    Print LinkedList elements
 *    Add 1 element from front - Succeed
 *    Print LinkedList elements
 *    Add 1 more element - it should Fail
 *    Delete 2 elements from front - one by one
 *    Print LinkedList elements
 *    Delete all remaining elements from front
 *    Print LinkedList elements - this should be empty
 *    Uninit the LinkedList
 *
 *****************************************************************************/

int Ut_Add_Delete_Elements_TestFunc02()
{
   int ok               = FALSE;
   int numElements      = -1;
   int index            = 0;

   int nodeData[LINKEDLISTLIB_MAX_NUM_NODES];

   ListNode *listHandle = NULL;

   LOG_MESSAGE("%s(): Starting ... ", __FUNCTION__);

   if (!LinkedListLib_InitList(&listHandle)) {
      LOG_MESSAGE("%s(): Error in Initializing LinkedList", __FUNCTION__);
      listHandle = NULL;
      goto exit;
   }

   LOG_MESSAGE("%s(): LinkedListLib_InitList(): Success, listHandle: 0x%p",
                                                            __FUNCTION__, listHandle);

   // ---------------------------------------------------------------------------
   // Add LINKEDLISTLIB_MAX_NUM_NODES from rear one-by-one
   // Print Number of elements
   // Print List Elements

   for(index = 0 ; index < LINKEDLISTLIB_MAX_NUM_NODES ; index++) {

      if (!LinkedListLib_AddIntDataNodeFromFront(listHandle, index)) {
         LOG_MESSAGE("%s(): Failed to add element \"%d\" from rear!", index);
         goto exit;
      }

   }

   if (!LinkedListLib_GetListNumElements(listHandle, &numElements)) {
      LOG_MESSAGE("%s(): Failed to get number of elements in the List!");
      goto exit;
   }
   LOG_MESSAGE("%s(): Number of elements in the List: %d",
                                          __FUNCTION__, numElements);


   if (!LinkedListLib_PrintListToLog(listHandle)) {
      LOG_MESSAGE("%s(): Failed to print List elements!", __FUNCTION__);
      goto exit;
   }


   // ---------------------------------------------------------------------------

   // ---------------------------------------------------------------------------
   // Add one more element from rear - it should Fail
   // Delete one element from read
   // Print Number of elements
   // Print List Elements
   // Add one element from rear - this should succeed
   // Print Number of elements
   // Print List Elements
   // Add one more element from rear - this should Fail

   if (LinkedListLib_AddIntDataNodeFromFront(listHandle, index)) {
      LOG_MESSAGE("%s(): This Node addition should have Failed!");
      goto exit;
   }

   if (!LinkedListLib_DeleteIntDataNodeFromFront(listHandle, &nodeData[index-1])) {
      LOG_MESSAGE("%s(): Failed to delete node from rear!", __FUNCTION__);
      goto exit;
   }

   if (!LinkedListLib_GetListNumElements(listHandle, &numElements)) {
      LOG_MESSAGE("%s(): Failed to get number of elements in the List!");
      goto exit;
   }
   LOG_MESSAGE("%s(): Number of elements in the List: %d",
                                          __FUNCTION__, numElements);

   if (!LinkedListLib_PrintListToLog(listHandle)) {
      LOG_MESSAGE("%s(): Failed to print List elements!", __FUNCTION__);
      goto exit;
   }

   if (!LinkedListLib_AddIntDataNodeFromFront(listHandle, index-1)) {
      LOG_MESSAGE("%s(): Failed to add element \"%d\" from rear!",
                                       __FUNCTION__, index);
      goto exit;
   }

   if (!LinkedListLib_GetListNumElements(listHandle, &numElements)) {
      LOG_MESSAGE("%s(): Failed to get number of elements in the List!");
      goto exit;
   }
   LOG_MESSAGE("%s(): Number of elements in the List: %d",
                                          __FUNCTION__, numElements);


   if (!LinkedListLib_PrintListToLog(listHandle)) {
      LOG_MESSAGE("%s(): Failed to print List elements!", __FUNCTION__);
      goto exit;
   }

   if (LinkedListLib_AddIntDataNodeFromFront(listHandle, index)) {
      LOG_MESSAGE("%s(): This Node addition should have Failed!");
      goto exit;
   }

   // ---------------------------------------------------------------------------

   // ---------------------------------------------------------------------------
   // Delete two elements from rear, one by one
   // Print Number of elements
   // Print List Elements

   if (!LinkedListLib_DeleteIntDataNodeFromFront(listHandle, &nodeData[index-1])) {
      LOG_MESSAGE("%s(): Failed to delete node from rear!", __FUNCTION__);
      goto exit;
   }

   if (!LinkedListLib_DeleteIntDataNodeFromFront(listHandle, &nodeData[index-2])) {
      LOG_MESSAGE("%s(): Failed to delete node from rear!", __FUNCTION__);
      goto exit;
   }

   if (!LinkedListLib_GetListNumElements(listHandle, &numElements)) {
      LOG_MESSAGE("%s(): Failed to get number of elements in the List!");
      goto exit;
   }
   LOG_MESSAGE("%s(): Number of elements in the List: %d",
                                          __FUNCTION__, numElements);

   if (!LinkedListLib_PrintListToLog(listHandle)) {
      LOG_MESSAGE("%s(): Failed to print List elements!", __FUNCTION__);
      goto exit;
   }

   index -= 2;

   // ---------------------------------------------------------------------------

   while (index > 0) {

      if (!LinkedListLib_DeleteIntDataNodeFromFront(listHandle, &nodeData[index])) {
         LOG_MESSAGE("%s(): Failed to delete node from rear!", __FUNCTION__);
         goto exit;
      }

      index -= 1;
   }

   if (!LinkedListLib_GetListNumElements(listHandle, &numElements)) {
      LOG_MESSAGE("%s(): Failed to get number of elements in the List!");
      goto exit;
   }
   LOG_MESSAGE("%s(): Number of elements in the List: %d",
                                          __FUNCTION__, numElements);

   if (!LinkedListLib_PrintListToLog(listHandle)) {
      LOG_MESSAGE("%s(): Failed to print List elements!", __FUNCTION__);
      goto exit;
   }

   // ---------------------------------------------------------------------------

   ok = TRUE;

exit:
   if (listHandle) {

      if (!LinkedListLib_UninitList(listHandle)) {
         LOG_MESSAGE("%s(): Error in Uninitializing LinkedList", __FUNCTION__);
         ok = FALSE;
      } else {
         LOG_MESSAGE("%s(): LinkedListLib_UninitList(): Success", __FUNCTION__);
      }

      listHandle = NULL;
   }

   LOG_MESSAGE("%s(): Test %s", __FUNCTION__, ok == TRUE ? "Succeeded" : "Failed");
   return ok;
}
