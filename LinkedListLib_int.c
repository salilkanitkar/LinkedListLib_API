#include <stdio.h>
#include <stdlib.h>

#include "LinkedListLib.h"
#include "LinkedListLib_int.h"
#include "LinkedListLib_Log.h"

LinkedListLibInst *theLinkedListLibInst = NULL;


/******************************************************************************
 * Function MainLinkedListLibInstExists --
 *    Checks if the Singleton global instance theLinkedListLibInst is
 *    already initialized or not
 *****************************************************************************/

int MainLinkedListLibInstExists()
{
   if (!theLinkedListLibInst) {
      printf("%s(): LinkedListLib not Initialized !", __FUNCTION__);
      printf("%s(): Please invoke LinkedListLib_Init() first.", __FUNCTION__);
      return FALSE;
   }

   return TRUE;
}


/******************************************************************************
 * Function LinkedListLibInstExists --
 *    Checks if the Singleton global instance theLinkedListLibInst is
 *    already initialized or not
 *****************************************************************************/

int LinkedListLibInstExists(LinkedListLibInst *linkedListLibInst)
{
   if (!linkedListLibInst) {
      printf("%s(): LinkedListLibInst not Initialized !", __FUNCTION__);
      printf("%s(): Please invoke LinkedListLib_Init() first.", __FUNCTION__);
      return FALSE;
   }

   return TRUE;
}


/******************************************************************************
 * Function InitHeadNodeList --
 *    Initialize the empty m_lists of theLinkedListLibInst
 *****************************************************************************/

int InitHeadNodeList(LinkedListLibInst *linkedListLibInst) // IN
{
   if (!LinkedListLibInstExists(linkedListLibInst)) {
      return FALSE;
   }

   LOG_DEBUG("Enter");
   int i = 0;

   for (i=0 ; i<LINKEDLISTLIB_MAX_NUM_LISTS ; i++) {
      linkedListLibInst->m_lists[i] = NULL;
   }

   linkedListLibInst->m_numLists = 0;
   LOG_DEBUG("Exit");
   return TRUE;
}


/******************************************************************************
 * Function UninitHeadNodeList --
 *    Uninitialize the m_lists of theLinkedListLibInst
 *****************************************************************************/

int UninitHeadNodeList(LinkedListLibInst *linkedListLibInst) // IN
{
   if (!LinkedListLibInstExists(linkedListLibInst)) {
      return TRUE;
   }

   LOG_DEBUG("Enter");
   int i = 0;

   for (i=0 ; i<LINKEDLISTLIB_MAX_NUM_LISTS ; i++) {

      if (linkedListLibInst->m_lists[i]) {

         // DeleteHeadNode() also destroys the LinkedList associated with it
         // - if present
         if (!DeleteHeadNode(theLinkedListLibInst, i)) {
            LOG_ERROR("DeleteHeadNode() failed for index: %d", i);
            return FALSE;
         }

      }

   }

   linkedListLibInst->m_numLists = 0;
   LOG_DEBUG("Exit");
   return TRUE;
}


/******************************************************************************
 * Function IsLinkedListLibInstFull --
 *****************************************************************************/

int IsLinkedListLibInstFull(LinkedListLibInst *linkedListLibInst) // IN
{
   if (!LinkedListLibInstExists(linkedListLibInst)) {
      return FALSE;
   }

   if (linkedListLibInst->m_numLists == LINKEDLISTLIB_MAX_NUM_LISTS) {
      return TRUE;
   }

   return FALSE;
}


/******************************************************************************
 * Function DeleteList --
 *    Delete LinkedList given a listHandle
 *****************************************************************************/
int DeleteList(ListNode *listHandle) // IN
{
   LOG_DEBUG("Enter");

   // TODO: Validate that listHandle is pointing to a headNode

   LOG_DEBUG("Deleting LinkedList: linkHandle=0x%p", listHandle);

   ListNode *node = listHandle->m_next;
   ListNode *temp = NULL;

   while (node) {
      temp = node;

      node = node->m_next;
      listHandle->m_next = node;

      free(temp);
      temp = NULL;
   }

   LOG_DEBUG("Exit");
   return TRUE;
}


/******************************************************************************
 * Function DeleteHeadNode --
 *    Deletes the headNode given an index
 *    Side Effect: If the headNode was pointing to a non-empty LinkedList,
 *                 then that LinkedList will also be deleted out
 *****************************************************************************/

int DeleteHeadNode(LinkedListLibInst *linkedListLibInst, // IN
                   int index)                            // IN
{
   if (!LinkedListLibInstExists(linkedListLibInst)) {
      return FALSE;
   }

   LOG_DEBUG("Enter");

   if (!linkedListLibInst->m_lists[index]) {
      LOG_ERROR("LinkedList at index %d is already deleted !", index);
      return FALSE;
   }

   /*
    * DeleteHeadNode() should ideally be invoked after DeleteList() on that
    * headNode is already done. However, just to confirm, invoke DeleteList()
    * nonetheless.
    */

   if (linkedListLibInst->m_lists[index]->m_next) {
      DeleteList(linkedListLibInst->m_lists[index]);
   }

   LOG_DEBUG("Deleting headNode 0x%p at index %d",
                linkedListLibInst->m_lists[index],
                index);

   free(linkedListLibInst->m_lists[index]);
   linkedListLibInst->m_lists[index] = NULL;
   linkedListLibInst->m_numLists -= 1;

   LOG_DEBUG("Exit");
   return TRUE;
}


/******************************************************************************
 * Function GetFirstHeadNodeEmptySlot --
 *****************************************************************************/
int GetFirstHeadNodeEmptySlot(LinkedListLibInst *linkedListLibInst, // IN
                              int *index)                           // OUT
{
   if (!LinkedListLibInstExists(linkedListLibInst)) {
      return FALSE;
   }

   if (!index) {
      LOG_ERROR("Invalid parameter - index is NULL");
      return FALSE;
   }

   LOG_DEBUG("Enter");

   int ok = FALSE;
   int i = 0;

   for (i=0 ; i < LINKEDLISTLIB_MAX_NUM_LISTS ; i++) {
      if (!linkedListLibInst->m_lists[i]) {
         *index = i;
         ok = TRUE;
         break;
      }
   }

   LOG_DEBUG("Exit");
   return ok;
}


/******************************************************************************
 * Function IsHeadNode --
 *****************************************************************************/

int IsHeadNode(ListNode *node) // IN
{
   int ok = FALSE;

   if (!MainLinkedListLibInstExists()) {
      return ok;
   }

   if (!node) {
      LOG_ERROR("Invalid args - node ptr is NULL");
      return ok;
   }

   ok = node->m_type == LIST_NODE_HEAD ? TRUE : FALSE;
   return ok;
}


/******************************************************************************
 * Function IsDataNode --
 *****************************************************************************/

int IsDataNode(ListNode *node) // IN
{
   int ok = FALSE;

   if (!MainLinkedListLibInstExists()) {
      return ok;
   }

   if (!node) {
      LOG_ERROR("Invalid args - node ptr is NULL");
      return ok;
   }

   ok = node->m_type == LIST_NODE_DATA ? TRUE : FALSE;
   return ok;
}


/******************************************************************************
 * Function IsListFull --
 *****************************************************************************/

int IsListFull(ListNode *listHandle) // IN
{
   int ok = FALSE;

   if (!MainLinkedListLibInstExists()) {
      return ok;
   }

   if (!listHandle) {
      LOG_ERROR("Invalid args - node ptr is NULL");
      return ok;
   }

   LOG_DEBUG("Enter - 0x%p", listHandle);

   if (!IsHeadNode(listHandle)) {
      LOG_ERROR("Invalid args - listHandle 0x%p is Not a Head node",
                                 listHandle);
      return ok;
   }

   ok = listHandle->m_data >= LINKEDLISTLIB_MAX_NUM_NODES ? TRUE : FALSE;
   LOG_DEBUG("listHandle 0x%p is %sFull",
                              listHandle,
                              ok == TRUE ? "" : "Not ");

   return ok;
}


/******************************************************************************
 * Function IsListEmpty --
 *****************************************************************************/

int IsListEmpty(ListNode *listHandle) // IN
{
   int ok = FALSE;

   if (!MainLinkedListLibInstExists()) {
      return ok;
   }

   if (!listHandle) {
      LOG_ERROR("Invalid args - node ptr is NULL");
      return ok;
   }

   if (!IsHeadNode(listHandle)) {
      LOG_ERROR("Invalid args - listHandle 0x%p is Not a Head node",
                                 listHandle);
      return FALSE;
   }

   ok = listHandle->m_data == 0 ? TRUE : FALSE;
   LOG_TRACE("listHandle 0x%p is %sEmpty", ok == TRUE ? "" : "Not ");

   return ok;
}
