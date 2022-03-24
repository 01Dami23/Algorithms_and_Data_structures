#ifndef LAB_8_QUOTATIONBST_H
#define LAB_8_QUOTATIONBST_H

#include <stdio.h>
#include <stdlib.h>
#include "quotation.h"

typedef struct binarysearchtree *BST;

BST BSTinit(void);
void BSTfree(BST bst);
quotation *BSTsearch(BST bst, dateTime k);
void BSTinsert_leafR(BST bst, quotation x);
void BSTbalance(BST bst);

void tree_print_wrapper(BST bst);
void BSTvisit_datesInterval(BST bst, dateTime dateTime1, dateTime dateTime2, int all);
int BSTminHeight(BST bst);
int BSTmaxHeight(BST bst);

void print2DBST_wrapper(BST bst);

#endif
