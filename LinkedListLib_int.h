#ifndef HEADER_LINKEDLISTLIB_INT_H // include guard
#define HEADER_LINKEDLISTLIB_INT_H

#include "LinkedListLib.h"
#include "LinkedListLib_Log.h"


/******************************************************************************
 * struct LinkedListLibInst --
 *    One LinkedListLibInst Obj per initialization of the Library.
 *    TODO: This should be a Singleton
 *****************************************************************************/

typedef struct _LinkedListLibInst {
   LinkedListLib_LogLevel m_logLevel;
   ListNode*              m_lists[LINKEDLISTLIB_MAX_NUM_LISTS];
   int                    m_numLists;
}LinkedListLibInst;

extern LinkedListLibInst *theLinkedListLibInst;

int MainLinkedListLibInstExists();
int LinkedListLibInstExists();

int InitHeadNodeList(LinkedListLibInst   *linkedListLibInst);
int UninitHeadNodeList(LinkedListLibInst *linkedListLibInst);

int IsLinkedListLibInstFull(LinkedListLibInst *linkedListLibInst);

int IsHeadNode(ListNode *node);
int IsDataNode(ListNode *node);

int GetFirstHeadNodeEmptySlot(LinkedListLibInst *linkedListLibInst, int *index);

int DeleteHeadNode(LinkedListLibInst    *linkedListLibInst, int index);

int DeleteList(ListNode  *listHandle);

int IsListFull(ListNode  *listHandle);
int IsListEmpty(ListNode *listHandle);

#endif // include guard
