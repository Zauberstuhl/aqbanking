/* This file is auto-generated from "cellphoneproduct.xml" by the typemaker
   tool of Gwenhywfar. 
   Do not edit this file -- all changes will be lost! */
#ifndef CELLPHONEPRODUCT_P_H
#define CELLPHONEPRODUCT_P_H

#include "cellphoneproduct_l.h"


struct AB_CELLPHONE_PRODUCT {
  GWEN_LIST_ELEMENT(AB_CELLPHONE_PRODUCT)
  int _usage;
  int _modified;

  char *id;
  char *providerName;
  char *productName;
  int allowFreeValue;
  AB_VALUE *minimumValue;
  AB_VALUE *maximumValue;
  AB_VALUE_LIST *values;
};










#endif /* CELLPHONEPRODUCT_P_H */
