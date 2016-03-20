#include <stdio.h>
#include <stdlib.h>

#include "LinkedListLib.h"
#include "LinkedListLib_int.h"
#include "LinkedListLib_Log.h"


/******************************************************************************
 * Function LinkedListLib_Init --
 *    This function should be invoked to initialize the LinkedListLib
 *****************************************************************************/

int LinkedListLib_Init()
{
   if (MainLinkedListLibInstExists()) {
      // If theLinkedListLibInst is already initialized, then
      // logging should be up as well

      LOG_DEBUG("LinkedListLib is already initialized!");
      return TRUE;
   }

   theLinkedListLibInst = (LinkedListLibInst *)malloc(sizeof *theLinkedListLibInst * 1);
   if (!theLinkedListLibInst) {
      printf("%s(): Memory Allocation Failed, just exiting out ... \n", __FUNCTION__);
      return FALSE;
   }

   theLinkedListLibInst->m_logLevel = LOG_LEVEL_DEFAULT;
   LOG_INFO("LinkedListLib Initializing.");

   LOG_DEBUG("Default Log level is: %s",
                     LinkedListLib_LogLevel_To_Str(theLinkedListLibInst->m_logLevel));

   // Init the Head node list maintained by theLinkedListLibInst
   if (InitHeadNodeList(theLinkedListLibInst) == FALSE) {
      LOG_ERROR("InitHeadNodeList() Failed !");
      return FALSE;
   }

   // LOG_LEVEL_DEFAULT is DEBUG, so the below TRACE shouldn't show up in Log
   LOG_TRACE("theLinkedListLibInst: [0x%p]", theLinkedListLibInst);

   // The below INFO log should show up though
   LOG_INFO("LinkedListLib Initialized");

   return TRUE;
}


/******************************************************************************
 * Function LinkedListLib_Uninit --
 *    This Uninit func for LinkedListLib
 *****************************************************************************/

int LinkedListLib_Uninit()
{
   LOG_INFO("LinkedListLib Uninitialing");

   if (MainLinkedListLibInstExists()) {

      // Uninit the Head node list maintained by theLinkedListLibInst
      if (UninitHeadNodeList(theLinkedListLibInst) == FALSE) {
         LOG_ERROR("UninitHeadNodeList() Failed !");
      }

      LOG_INFO("LinkedListLib Uninitialized");
      free(theLinkedListLibInst);
      theLinkedListLibInst = NULL;
   }

   return TRUE;
}


/******************************************************************************
 * Function LinkedListLib_InitList --
 *    Initializes head node for the List - return pointer to this head node.
 *    This head node pointer can be considered as an equivalent of "handle" to
 *    the LinkedList.
 *
 *    The caller should record this headPointer/listHandle and it should be
 *    used to make any subsequent operations on this LinkedList.
 *****************************************************************************/

int LinkedListLib_InitList(ListNode **listHandle)
{
   if (!MainLinkedListLibInstExists()) {
      return FALSE;
   }

   LOG_DEBUG("Enter");

   if (IsLinkedListLibInstFull(theLinkedListLibInst)) {
      LOG_INFO("Can not create a new LinkedList");
      LOG_INFO("theLinkedListLibInst alreafy full");
      return FALSE;
   }

   ListNode *node = (ListNode *)malloc(sizeof *node * 1);
   if (!node) {
      LOG_ERROR("Memory allocation Failed!");
      return FALSE;
   }

   // Head node's m_data will keep track of number of Nodes in the list
   node->m_type = LIST_NODE_HEAD;
   node->m_data = 0;
   node->m_next = NULL;

   /*
    * Record the listHandle
    */

   int index;
   if (!GetFirstHeadNodeEmptySlot(theLinkedListLibInst, &index)) {
      LOG_ERROR("Failed to get an empty slot for headNode");
      return FALSE;
   }
   LOG_DEBUG("New LinkedList will be at index: %d", index);

   theLinkedListLibInst->m_lists[index] = node;
   theLinkedListLibInst->m_numLists += 1;

   LOG_DEBUG("Added listHandle: 0x%p", node);
   LOG_DEBUG("Number of Lists: %d", theLinkedListLibInst->m_numLists);

   /*
    * Return pointer to head node back to the caller
    * The caller is expected to record this head pointer
    * and it should be used to make any subsequent operations
    * on this LinkedList
    */
   *listHandle = node;

   LOG_DEBUG("Exit");
   return TRUE;
}


/******************************************************************************
 * Function LinkedListLib_UninitList --
 *    Delete every Data node of the list. Delete head node.
 *****************************************************************************/

int LinkedListLib_UninitList(ListNode *listHandle)
{
   if (!MainLinkedListLibInstExists()) {
      return FALSE;
   }

   LOG_DEBUG("Enter - listHandle: 0x%p", listHandle);

   int index;

   if (!LinkedListLib_ListExists(listHandle, &index)) {
      LOG_ERROR("LinkedList 0x%p does not Exist in LinkedListLib", listHandle);
      return FALSE;
   }
   LOG_DEBUG("LinkedList found at index: %d", index);

   if (!DeleteList(listHandle)) {
      LOG_ERROR("DeleteList() failed for listHandle: 0x%p", listHandle);
      return FALSE;
   }

   if (!DeleteHeadNode(theLinkedListLibInst, index)) {
      LOG_ERROR("DeleteHeadNode() failed for index: %d", index);
      return FALSE;
   }

   LOG_DEBUG("LinkedList deleted, theLinkedListLibInst->m_numLists: %d",
                                  theLinkedListLibInst->m_numLists);

   LOG_DEBUG("Exit");
   return TRUE;
}


/******************************************************************************
 * Function LinkedListLib_ListExists --
 *    Checks if the list corresponding to given listHandle exists or not
 *****************************************************************************/

int LinkedListLib_ListExists(ListNode *listHandle, // IN
                             int      *retIndex)   // OUT
{
   if (!MainLinkedListLibInstExists()) {
      return FALSE;
   }

   LOG_DEBUG("Enter - listHandle: 0x%p", listHandle);

   if (!retIndex) {
      LOG_ERROR("Invalid Parameter - *retIndex is NULL");
      return FALSE;
   }

   int ok = FALSE;
   int i = 0;

   for (i=0 ; i < LINKEDLISTLIB_MAX_NUM_LISTS ; i++) {

        if (theLinkedListLibInst->m_lists[i] == listHandle) {
         LOG_DEBUG("LinkedList found in theLinkedListLib at index %d", i);
         *retIndex = i;
         ok = TRUE;
         break;
      }

   }

   LOG_DEBUG("Exit");
   return ok;
}


/******************************************************************************
 * Function LinkedListLib_IsFull --
 *****************************************************************************/

int LinkedListLib_IsFull()
{
   if (!MainLinkedListLibInstExists()) {
      return FALSE;
   }

   return IsLinkedListLibInstFull(theLinkedListLibInst);
}


/******************************************************************************
 * Function LinkedListLib_IsFullList --
 *****************************************************************************/

int LinkedListLib_IsFullList(ListNode  *listHandle) // IN
{
   int ok = FALSE;

   if (!MainLinkedListLibInstExists()) {
      return ok;
   }

   int retIndex = -1;

   if (!LinkedListLib_ListExists(listHandle, &retIndex)) {
      LOG_ERROR("LinkedList with Handle 0x%p does Not exist", listHandle);
      return ok;
   }

   LOG_DEBUG("About to check if LinkedList [0x%p] is Full or Not ...",
                                 listHandle);

   ok = (IsListFull(listHandle) == TRUE ? TRUE : FALSE);
   return ok;
}


/******************************************************************************
 * Function LinkedListLib_IsEmptyList --
 *****************************************************************************/

int LinkedListLib_IsEmptyList(ListNode  *listHandle) // IN
{
   int ok = FALSE;

   if (!MainLinkedListLibInstExists()) {
      return ok;
   }

   int retIndex = -1;

   if (!LinkedListLib_ListExists(listHandle, &retIndex)) {
      LOG_ERROR("LinkedList with Handle 0x%p does Not exist", listHandle);
      return ok;
   }

   LOG_DEBUG("About to check if LinkedList [0x%p] is Empty or Not ...",
                                 listHandle);

   ok = (IsListEmpty(listHandle) == TRUE ? TRUE : FALSE);
   return ok;
}


/******************************************************************************
 * Function LinkedListLib_GetListNumElements --
 *****************************************************************************/

int LinkedListLib_GetListNumElements(ListNode *listHandle,  // IN
                                     int      *numElements) // OUT
{
   int ok       = FALSE;
   int retIndex = -1;

   if (!numElements) {
      LOG_ERROR("Invalid args - NULL second parameter!");
      goto exit;
   }

   if (!LinkedListLib_ListExists(listHandle, &retIndex)) {
      LOG_ERROR("Inavlid args - LinkedList 0x%p does Not exist",
                                       listHandle);
      goto exit;
   }

   if (!IsHeadNode(listHandle)) {
      LOG_ERROR("Invalid args - listHandle 0x%p is corrupt",
                                       listHandle);
      goto exit;
   }

   *numElements = listHandle->m_data;
   LOG_DEBUG("Number of Elements in LinkedList[%d] = %d",
                                       retIndex, *numElements);

   ok = TRUE;

exit:
   LOG_DEBUG("Getting Number of elements in the List: %s",
                              ok == TRUE ? "Succeeded" : "Failed!");
   return ok;
}


/******************************************************************************
 * Function LinkedListLib_AddIntDataNodeFromRear --
 *****************************************************************************/

int LinkedListLib_AddIntDataNodeFromRear(ListNode   *listHandle, // IN
                                         int        nodeData)    // IN
{
   int ok       = FALSE;
   int retIndex = -1;

   if (!LinkedListLib_ListExists(listHandle, &retIndex)) {
      goto exit;
   }

   if (LinkedListLib_IsFullList(listHandle)) {
      goto exit;
   }

   LOG_DEBUG("Adding element from rear to LinkedList[%d]", retIndex);

   ListNode *tmp = listHandle->m_next;
   while (tmp && tmp->m_next) {
      tmp = tmp->m_next;
   }

   ListNode *newNode = NULL;

   newNode = (ListNode *)malloc(sizeof(*newNode) * 1);
   if (!newNode) {
      LOG_ERROR("Memory allocation Failed!");
      goto exit;
   }

   newNode->m_type = LIST_NODE_DATA;
   newNode->m_data = nodeData;
   newNode->m_next = NULL;

   if (tmp) {
      tmp->m_next = newNode;
   } else {
      listHandle->m_next = newNode;
   }

   listHandle->m_data += 1;

   ok = TRUE;

exit:
   LOG_DEBUG("Node addition to rear of LinkedList[%d]: %s",
                                 retIndex,
                                 ok == TRUE ? "Succeeded" : "Failed!");
   return ok;
}


/******************************************************************************
 * Function LinkedListLib_DeleteIntDataNodeFromRear --
 *****************************************************************************/

int LinkedListLib_DeleteIntDataNodeFromRear(ListNode   *listHandle, // IN
                                            int        *nodeData)   // OUT
{
   int ok       = FALSE;
   int retIndex = -1;

   if (!nodeData) {
      goto exit;
   }

   if (!LinkedListLib_ListExists(listHandle, &retIndex)) {
      goto exit;
   }

   if (LinkedListLib_IsEmptyList(listHandle)) {
      goto exit;
   }

   LOG_DEBUG("Deleting element from rear to LinkedList[%d]", retIndex);

   ListNode *last, *newLast;

   last    = listHandle->m_next;
   newLast = listHandle;

   while (last->m_next) {
      last    = last->m_next;
      newLast = newLast->m_next;
   }

   *nodeData = last->m_data;
   free(last);

   newLast->m_next = NULL;
   listHandle->m_data -= 1;

   LOG_DEBUG("Node %d deleted from LinkedListLib[%d]", *nodeData, retIndex);
   ok = TRUE;

exit:
      LOG_DEBUG("Node deletion from rear of LinkedList[%d]: %s",
                                    retIndex,
                                    ok == TRUE ? "Succeeded" : "Failed!");
      return ok;
}


/******************************************************************************
 * Function LinkedListLib_AddIntDataNodeFromFront --
 *****************************************************************************/

int LinkedListLib_AddIntDataNodeFromFront(ListNode  *listHandle,  // IN
                                          int       nodeData)     // IN
{
   int ok       = FALSE;
   int retIndex = -1;

   if (!LinkedListLib_ListExists(listHandle, &retIndex)) {
      goto exit;
   }

   if (LinkedListLib_IsFullList(listHandle)) {
      goto exit;
   }

   LOG_DEBUG("Adding element from front to LinkedList[%d]", retIndex);

   ListNode *tmp     = listHandle->m_next;
   ListNode *newNode = NULL;

   newNode = (ListNode *)malloc(sizeof(*newNode) * 1);
   if (!newNode) {
      LOG_ERROR("Memory allocation Failed!");
      goto exit;
   }

   newNode->m_type = LIST_NODE_DATA;
   newNode->m_data = nodeData;
   newNode->m_next = tmp;

   listHandle->m_next = newNode;
   listHandle->m_data += 1;

   ok = TRUE;

exit:
   LOG_DEBUG("Node addition to rear of LinkedList[%d]: %s",
                                 retIndex,
                                 ok == TRUE ? "Succeeded" : "Failed!");
   return ok;
}


/******************************************************************************
 * Function LinkedListLib_DeleteIntDataNodeFromFront --
 *****************************************************************************/
int LinkedListLib_DeleteIntDataNodeFromFront(ListNode  *listHandle, // IN
                                             int       *nodeData)   // OUT
{
   int ok       = FALSE;
   int retIndex = -1;

   if (!nodeData) {
      goto exit;
   }

   if (!LinkedListLib_ListExists(listHandle, &retIndex)) {
      goto exit;
   }

   if (LinkedListLib_IsEmptyList(listHandle)) {
      goto exit;
   }

   LOG_DEBUG("Deleting element from front to LinkedList[%d]", retIndex);

   ListNode *tmp = listHandle->m_next;
   if (tmp->m_next) {
      listHandle->m_next = tmp->m_next;
   } else {
      listHandle->m_next = NULL;
   }

   *nodeData  = tmp->m_data;
   free(tmp);

   listHandle->m_data -= 1;
   LOG_DEBUG("Node %d deleted from LinkedListLib[%d]", *nodeData, retIndex);

   ok = TRUE;

exit:
      LOG_DEBUG("Node deletion from front of LinkedList[%d]: %s",
                                    retIndex,
                                    ok == TRUE ? "Succeeded" : "Failed!");
      return ok;
}


/******************************************************************************
 * Function LinkedListLib_PrintHeadNodes --
 *****************************************************************************/

int LinkedListLib_PrintHeadNodes()
{
   int i = 0;

   LOG_DEBUG("Enter - theLinkedListLibInst->m_numLists: %d", theLinkedListLibInst->m_numLists);

   for(i = 0 ; i < theLinkedListLibInst->m_numLists ; i++) {
      LOG_DEBUG("theLinkedListLibInst->m_lists[i]: 0x%p", theLinkedListLibInst->m_lists[i]);
   }

   return TRUE;
}


/******************************************************************************
 * Function LinkedListLib_PrintListToLog --
 *****************************************************************************/

int LinkedListLib_PrintListToLog(ListNode *listHandle) // IN
{
   int ok       = FALSE;
   int retIndex = -1;

   if (!LinkedListLib_ListExists(listHandle, &retIndex)) {
      LOG_ERROR("Invalid args - List 0x%p does Not exists", listHandle);
      goto exit;
   }

   if (LinkedListLib_IsEmptyList(listHandle)) {
      LOG_DEBUG("LinkedList[%d][0x%p] is Empty ... ", retIndex, listHandle);
      ok = TRUE;
      goto exit;
   }

   ListNode *tmp = listHandle->m_next;

   while (tmp) {
      LOG_DEBUG("%d ->", tmp->m_data);
      tmp = tmp->m_next;
   }
   LOG_DEBUG("NULL");

   ok = TRUE;

exit:
   return ok;
}
