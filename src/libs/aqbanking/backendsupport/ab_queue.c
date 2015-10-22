/**********************************************************
 * This file has been automatically created by "typemaker2"
 * from the file "ab_queue.xml".
 * Please do not edit this file, all changes will be lost.
 * Better edit the mentioned source file instead.
 **********************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "ab_queue_p.h"

#include <gwenhywfar/misc.h>
#include <gwenhywfar/debug.h>

/* code headers */
#include "ab_queuefns.c"

/* macro functions */
GWEN_INHERIT_FUNCTIONS(AB_QUEUE)


AB_QUEUE *AB_Queue_new(void) {
  AB_QUEUE *p_struct;

  GWEN_NEW_OBJECT(AB_QUEUE, p_struct)
  p_struct->_refCount=1;
  GWEN_INHERIT_INIT(AB_QUEUE, p_struct)
  /* members */
  p_struct->userQueueList=AB_UserQueue_List_new();
  /* virtual functions */

  return p_struct;
}

void AB_Queue_free(AB_QUEUE *p_struct) {
  if (p_struct) {
  assert(p_struct->_refCount);
  if (p_struct->_refCount==1) {
    GWEN_INHERIT_FINI(AB_QUEUE, p_struct)
  /* members */
    AB_UserQueue_List_free(p_struct->userQueueList);
    p_struct->_refCount=0;
    GWEN_FREE_OBJECT(p_struct);
  }
  else
    p_struct->_refCount--;
  }
}

void AB_Queue_Attach(AB_QUEUE *p_struct) {
  assert(p_struct);
  assert(p_struct->_refCount);
  p_struct->_refCount++;
}

AB_QUEUE *AB_Queue_dup(const AB_QUEUE *p_src) {
  AB_QUEUE *p_struct;

  assert(p_src);
  p_struct=AB_Queue_new();
  /* member "userQueueList" */
  if (p_struct->userQueueList) {
    AB_UserQueue_List_free(p_struct->userQueueList);
    p_struct->userQueueList=NULL;
  }
  if (p_src->userQueueList) {
    p_struct->userQueueList=AB_UserQueue_List_dup(p_src->userQueueList);
  }

  return p_struct;
}

AB_QUEUE *AB_Queue_copy(AB_QUEUE *p_struct, const AB_QUEUE *p_src) {
    assert(p_struct);
  assert(p_src);
  /* member "userQueueList" */
  if (p_struct->userQueueList) {
    AB_UserQueue_List_free(p_struct->userQueueList);
  }
  if (p_struct->userQueueList) {
    AB_UserQueue_List_free(p_struct->userQueueList);
    p_struct->userQueueList=NULL;
  }
  if (p_src->userQueueList) {
    p_struct->userQueueList=AB_UserQueue_List_dup(p_src->userQueueList);
  }

  return p_struct;
}

AB_USERQUEUE_LIST *AB_Queue_GetUserQueueList(const AB_QUEUE *p_struct) {
  assert(p_struct);
  return p_struct->userQueueList;
}

void AB_Queue_SetUserQueueList(AB_QUEUE *p_struct, AB_USERQUEUE_LIST *p_src) {
  assert(p_struct);
  if (p_struct->userQueueList) {
    AB_UserQueue_List_free(p_struct->userQueueList);
  }
  p_struct->userQueueList=p_src;
}


/* code headers */

