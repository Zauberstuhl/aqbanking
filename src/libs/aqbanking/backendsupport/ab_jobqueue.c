/**********************************************************
 * This file has been automatically created by "typemaker2"
 * from the file "ab_jobqueue.xml".
 * Please do not edit this file, all changes will be lost.
 * Better edit the mentioned source file instead.
 **********************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "ab_jobqueue_p.h"

#include <gwenhywfar/misc.h>
#include <gwenhywfar/debug.h>

/* code headers */

/* macro functions */
GWEN_LIST_FUNCTIONS(AB_JOBQUEUE, AB_JobQueue)
GWEN_INHERIT_FUNCTIONS(AB_JOBQUEUE)


AB_JOBQUEUE *AB_JobQueue_new(void) {
  AB_JOBQUEUE *p_struct;

  GWEN_NEW_OBJECT(AB_JOBQUEUE, p_struct)
  p_struct->_refCount=1;
  GWEN_INHERIT_INIT(AB_JOBQUEUE, p_struct)
  GWEN_LIST_INIT(AB_JOBQUEUE, p_struct)
  /* members */
  p_struct->jobList=AB_Job_List2_new();
  p_struct->jobType=AB_Job_TypeUnknown;
  /* virtual functions */

  return p_struct;
}

void AB_JobQueue_free(AB_JOBQUEUE *p_struct) {
  if (p_struct) {
  assert(p_struct->_refCount);
  if (p_struct->_refCount==1) {
    GWEN_INHERIT_FINI(AB_JOBQUEUE, p_struct)
    GWEN_LIST_FINI(AB_JOBQUEUE, p_struct)
  /* members */
    AB_Job_List2_free(p_struct->jobList);
    p_struct->_refCount=0;
    GWEN_FREE_OBJECT(p_struct);
  }
  else
    p_struct->_refCount--;
  }
}

void AB_JobQueue_Attach(AB_JOBQUEUE *p_struct) {
  assert(p_struct);
  assert(p_struct->_refCount);
  p_struct->_refCount++;
}

AB_JOBQUEUE *AB_JobQueue_dup(const AB_JOBQUEUE *p_src) {
  AB_JOBQUEUE *p_struct;

  assert(p_src);
  p_struct=AB_JobQueue_new();
  /* member "jobList" */
  if (p_struct->jobList) {
    AB_Job_List2_free(p_struct->jobList);
    p_struct->jobList=NULL;
  }
  if (p_src->jobList) {
    
  }

  /* member "jobType" */
  p_struct->jobType=p_src->jobType;

  return p_struct;
}

AB_JOB_LIST2 *AB_JobQueue_GetJobList(const AB_JOBQUEUE *p_struct) {
  assert(p_struct);
  return p_struct->jobList;
}

int AB_JobQueue_GetJobType(const AB_JOBQUEUE *p_struct) {
  assert(p_struct);
  return p_struct->jobType;
}

void AB_JobQueue_SetJobList(AB_JOBQUEUE *p_struct, AB_JOB_LIST2 *p_src) {
  assert(p_struct);
  if (p_struct->jobList) {
    AB_Job_List2_free(p_struct->jobList);
  }
  p_struct->jobList=p_src;
}

void AB_JobQueue_SetJobType(AB_JOBQUEUE *p_struct, int p_src) {
  assert(p_struct);
  p_struct->jobType=p_src;
}

/* list1 functions */
AB_JOBQUEUE_LIST *AB_JobQueue_List_dup(const AB_JOBQUEUE_LIST *p_src) {
  AB_JOBQUEUE_LIST *p_dest;
  AB_JOBQUEUE *p_elem;

  assert(p_src);
  p_dest=AB_JobQueue_List_new();
  p_elem=AB_JobQueue_List_First(p_src);
  while(p_elem) {
    AB_JOBQUEUE *p_cpy;

    p_cpy=AB_JobQueue_dup(p_elem);
    AB_JobQueue_List_Add(p_cpy, p_dest);
    p_elem=AB_JobQueue_List_Next(p_elem);
  }

  return p_dest;
}


/* code headers */

