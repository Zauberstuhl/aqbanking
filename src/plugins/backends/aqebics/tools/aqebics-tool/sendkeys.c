/***************************************************************************
 $RCSfile: createkeys.c,v $
 -------------------
 cvs         : $Id: createkeys.c,v 1.2 2006/01/13 13:59:59 cstim Exp $
 begin       : Tue May 03 2005
 copyright   : (C) 2005 by Martin Preuss
 email       : martin@libchipcard.de

 ***************************************************************************
 *          Please see toplevel file COPYING for license details           *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif


#include "globals.h"

#include <gwenhywfar/text.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>


int sendKeys(AB_BANKING *ab,
             GWEN_DB_NODE *dbArgs,
             int argc,
             char **argv) {
  GWEN_DB_NODE *db;
  AB_PROVIDER *pro;
  AB_USER_LIST2 *ul;
  AB_USER *u=0;
  int rv;
  const char *bankId;
  const char *userId;
  const char *customerId;
  uint32_t guiid;
  const GWEN_ARGS args[]={
  {
    GWEN_ARGS_FLAGS_HAS_ARGUMENT, /* flags */
    GWEN_ArgsType_Char,           /* type */
    "bankId",                     /* name */
    0,                            /* minnum */
    1,                            /* maxnum */
    "b",                          /* short option */
    "bank",                       /* long option */
    "Specify the bank code",      /* short description */
    "Specify the bank code"       /* long description */
  },
  {
    GWEN_ARGS_FLAGS_HAS_ARGUMENT, /* flags */
    GWEN_ArgsType_Char,           /* type */
    "userId",                     /* name */
    0,                            /* minnum */
    1,                            /* maxnum */
    "u",                          /* short option */
    "user",                       /* long option */
    "Specify the user id",        /* short description */
    "Specify the user id"         /* long description */
  },
  {
    GWEN_ARGS_FLAGS_HAS_ARGUMENT, /* flags */
    GWEN_ArgsType_Char,           /* type */
    "customerId",                 /* name */
    0,                            /* minnum */
    1,                            /* maxnum */
    "c",                          /* short option */
    "customer",                   /* long option */
    "Specify the customer id (Kundennummer)",    /* short description */
    "Specify the customer id (Kundennummer)"     /* long description */
  },
  {
    GWEN_ARGS_FLAGS_HELP | GWEN_ARGS_FLAGS_LAST, /* flags */
    GWEN_ArgsType_Int,            /* type */
    "help",                       /* name */
    0,                            /* minnum */
    0,                            /* maxnum */
    "h",                          /* short option */
    "help",                       /* long option */
    "Show this help screen",      /* short description */
    "Show this help screen"       /* long description */
  }
  };

  db=GWEN_DB_GetGroup(dbArgs, GWEN_DB_FLAGS_DEFAULT, "local");
  rv=GWEN_Args_Check(argc, argv, 1,
                     0 /*GWEN_ARGS_MODE_ALLOW_FREEPARAM*/,
                     args,
                     db);
  if (rv==GWEN_ARGS_RESULT_ERROR) {
    fprintf(stderr, "ERROR: Could not parse arguments\n");
    return 1;
  }
  else if (rv==GWEN_ARGS_RESULT_HELP) {
    GWEN_BUFFER *ubuf;

    ubuf=GWEN_Buffer_new(0, 1024, 0, 1);
    if (GWEN_Args_Usage(args, ubuf, GWEN_ArgsOutType_Txt)) {
      fprintf(stderr, "ERROR: Could not create help string\n");
      return 1;
    }
    fprintf(stdout, "%s\n", GWEN_Buffer_GetStart(ubuf));
    GWEN_Buffer_free(ubuf);
    return 0;
  }

  rv=AB_Banking_Init(ab);
  if (rv) {
    DBG_ERROR(0, "Error on init (%d)", rv);
    return 2;
  }

  rv=AB_Banking_OnlineInit(ab);
  if (rv) {
    DBG_ERROR(0, "Error on onlineInit (%d)", rv);
    return 2;
  }

  pro=AB_Banking_GetProvider(ab, EBC_PROVIDER_NAME);
  assert(pro);

  bankId=GWEN_DB_GetCharValue(db, "bankId", 0, "*");
  userId=GWEN_DB_GetCharValue(db, "userId", 0, "*");
  customerId=GWEN_DB_GetCharValue(db, "customerId", 0, "*");

  ul=AB_Banking_FindUsers(ab, EBC_PROVIDER_NAME,
                          "de", bankId, userId, customerId);
  if (ul) {
    if (AB_User_List2_GetSize(ul)!=1) {
      DBG_ERROR(0, "Ambiguous customer specification");
      AB_Banking_OnlineFini(ab);
      AB_Banking_Fini(ab);
      return 3;
    }
    else {
      AB_USER_LIST2_ITERATOR *cit;

      cit=AB_User_List2_First(ul);
      assert(cit);
      u=AB_User_List2Iterator_Data(cit);
      AB_User_List2Iterator_free(cit);
    }
    AB_User_List2_free(ul);
  }
  if (!u) {
    DBG_ERROR(0, "No matching customer");
    AB_Banking_OnlineFini(ab);
    AB_Banking_Fini(ab);
    return 3;
  }
  else {
    guiid=GWEN_Gui_ProgressStart(GWEN_GUI_PROGRESS_ALLOW_SUBLEVELS |
				 GWEN_GUI_PROGRESS_SHOW_PROGRESS |
				 GWEN_GUI_PROGRESS_SHOW_LOG |
				 GWEN_GUI_PROGRESS_ALWAYS_SHOW_LOG |
				 GWEN_GUI_PROGRESS_KEEP_OPEN |
				 GWEN_GUI_PROGRESS_SHOW_ABORT,
				 I18N("Executing Request"),
				 I18N("Now the request is send "
				      "to the credit institute."),
				 GWEN_GUI_PROGRESS_NONE,
				 0);
    if (!(EBC_User_GetFlags(u) & EBC_USER_FLAGS_INI)) {
      rv=EBC_Provider_Send_INI(pro, u, 1);
      if (rv) {
        DBG_ERROR(0, "Error sending INI request (%d)", rv);
	GWEN_Gui_ProgressEnd(guiid);
	AB_Banking_OnlineFini(ab);
        AB_Banking_Fini(ab);
        return 4;
      }
      else {
	fprintf(stderr, "INI request sent.\n");
      }
    }
    if (!(EBC_User_GetFlags(u) & EBC_USER_FLAGS_HIA)) {
      rv=EBC_Provider_Send_HIA(pro, u, 1);
      if (rv) {
        DBG_ERROR(0, "Error sending HIA request (%d)", rv);
	GWEN_Gui_ProgressEnd(guiid);
	AB_Banking_OnlineFini(ab);
        AB_Banking_Fini(ab);
        return 4;
      }
      else {
	fprintf(stderr, "HIA request sent.\n");
      }
    }
    GWEN_Gui_ProgressEnd(guiid);
  }

  rv=AB_Banking_OnlineFini(ab);
  if (rv) {
    fprintf(stderr, "ERROR: Error on online deinit (%d)\n", rv);
    AB_Banking_Fini(ab);
    return 5;
  }

  rv=AB_Banking_Fini(ab);
  if (rv) {
    fprintf(stderr, "ERROR: Error on deinit (%d)\n", rv);
    return 5;
  }

  fprintf(stderr, "INI/HIA request ok.\n");

  return 0;
}




