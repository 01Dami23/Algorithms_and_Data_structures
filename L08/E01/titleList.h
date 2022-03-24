#ifndef LAB_8_TITLELIST_H
#define LAB_8_TITLELIST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "title.h"

typedef struct titleList_s *titleList_t;

titleList_t LISTinit(void);
void LISTfree (titleList_t titleList);
titleList_t titleList_addTitle(titleList_t titleList, char name[MAXCHAR]);
title_t titleList_searchByTitleName(titleList_t titleList, char name[MAXCHAR]);
void titleList_print(titleList_t titleList);

#endif
