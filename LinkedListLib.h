#ifndef HEADER_LINKEDLISTLIB_H // include guard
#define HEADER_LINKEDLISTLIB_H

#define TRUE  1
#define FALSE 0

// The LinkedListLib will manage a maximum of LINKEDLISTLIB_MAX_NUM_LISTS
// number of Linked Lists.
#define LINKEDLISTLIB_MAX_NUM_LISTS   10

// Each LinkedList will have a maximum of LINKEDLISTLIB_MAX_NUM_NODES data
// nodes (plus one head node)
// #define LINKEDLISTLIB_MAX_NUM_NODES   1024
#define LINKEDLISTLIB_MAX_NUM_NODES   10


/******************************************************************************
 * enum ListNodeType --
 *    A LinkedList will have exactly one head node and a maximum of
 *    LINKEDLISTLIB_MAX_NUM_NODES data nodes.
 *****************************************************************************/

typedef enum _ListNodeType {
   LIST_NODE_HEAD,
   LIST_NODE_DATA
}ListNodeType;


/******************************************************************************
 * struct ListNode --
 *****************************************************************************/

typedef struct _ListNode {
   struct _ListNode *m_next;
   int               m_data;
   ListNodeType      m_type;
}ListNode;


int LinkedListLib_Init();
int LinkedListLib_Uninit();

int LinkedListLib_InitList(ListNode  **listHandle);
int LinkedListLib_UninitList(ListNode *listHandle);

int LinkedListLib_ListExists(ListNode *listHandle, int *retIndex);
int LinkedListLib_IsFull();

int LinkedListLib_IsFullList(ListNode  *listHandle);
int LinkedListLib_IsEmptyList(ListNode *listHandle);

int LinkedListLib_PrintListToLog(ListNode *listHandle);

int LinkedListLib_GetListNumElements(ListNode *listHandle,
                                     int      *numElements);

int LinkedListLib_AddIntDataNodeFromRear(ListNode      *listHandle,
                                         int            nodeData);
int LinkedListLib_DeleteIntDataNodeFromRear(ListNode   *listHandle,
                                            int        *nodeData);

int LinkedListLib_AddIntDataNodeFromFront(ListNode  *listHandle,
                                          int        nodeData);
int LinkedListLib_DeleteIntDataNodeFromFront(ListNode  *listHandle,
                                             int       *nodeData);
int LinkedListLib_PrintHeadNodes();

#endif // include guard
