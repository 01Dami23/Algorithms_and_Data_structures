#include "quotationBST.h"

typedef struct BSTnode* link;
struct BSTnode {
    quotation quot;
    link p;
    link l;
    link r;
    int N;
};

struct binarysearchtree {
    link root;
    link z;
};

static link NEW(quotation quot, link p, link l, link r, int N);
static void treeFree(link h, link z);
static quotation *searchR(link h, dateTime k, link z);
static link insertR(link h, quotation x, link z);
static link rotR(link h);
static link rotL(link h);
static void treePrintR(link h, link z);
static link partR(link h, int r);
static link balanceR(link h, link z);

static void BSTvisit_datesIntervalR(link root, int date1, int date2, quotation *max, quotation *min, int *flag1_p, int *flag2_p);
static void BSTminHeightR(link root, int *min_p, int *len_p);
static void BSTmaxHeightR(link root, int *max_p, int *len_p);
static void checkErroreAllocazione(void *p);


static link NEW(quotation quot, link p, link l, link r, int N) {
    link x = malloc(sizeof *x);
    checkErroreAllocazione(x);
    x->quot = quot; x->p = p; x->l = l; x->r = r; x->N = N;
    return x;
}


BST BSTinit( ) {
    BST bst = malloc(sizeof *bst);
    checkErroreAllocazione(bst);
    bst->root = ( bst->z = NEW(QUOTATIONsetNull(), NULL, NULL, NULL, 0));
    return bst;
}


void BSTfree(BST bst) {
    if (bst == NULL)
        return;
    treeFree(bst->root, bst->z);
    free(bst->z);
    free(bst);
}


static void treeFree(link h, link z) {
    if (h == z)
        return;
    treeFree(h->l, z);
    treeFree(h->r, z);
    free(h);
}


static quotation *searchR(link h, dateTime k, link z) {
    int cmp;
    if (h == z)
        return NULL;
    cmp = quotation_DATEcmp(k, h->quot.dataOra);
    if (cmp==0)
        return &(h->quot);
    if (cmp==-1)
        return searchR(h->l, k, z);
    else
        return searchR(h->r, k, z);
}

quotation *BSTsearch(BST bst, dateTime k) {
    return searchR(bst->root, k, bst->z);
}


static link insertR(link h, quotation x, link z) {
    if (h == z)
        return NEW(x, z, z, z, 1);
    if (quotation_DATEcmp(x.dataOra, h->quot.dataOra) < 0) {
        h->l = insertR(h->l, x, z);
        h->l->p = h;
    }
    else {
        h->r = insertR(h->r, x, z);
        h->r->p = h;
    }
    h->N++;
    return h;
}


void BSTinsert_leafR(BST bst, quotation x) {
    bst->root = insertR(bst->root, x, bst->z);
}


static link rotR(link h) {
    link x = h->l;
    h->l = x->r;
    x->r->p = h;
    x->r = h;
    x->p = h->p;
    h->p = x;
    x->N = h->N;
    h->N = 1;
    h->N += h->l->N;
    h->N += h->r->N;
    return x;
}


static link rotL(link h) {
    link x = h->r;
    h->r = x->l;
    x->l->p = h;
    x->l = h;
    x->p = h->p;
    h->p = x;
    x->N = h->N;
    h->N = 1;
    h->N += h->l->N;
    h->N += h->r->N;
    return x;
}


static void treePrintR(link h, link z) {
    if (h == z)
        return;

    treePrintR(h->l, z);
    treePrintR(h->r, z);
    QUOTATIONstore(h->quot);
}


void tree_print_wrapper(BST bst) {
    treePrintR(bst->root, bst->z);
}


static void BSTvisit_datesIntervalR(link root, int date1, int date2, quotation *max, quotation *min, int *flag1_p, int *flag2_p) {
    quotation current;
    int current_date;

    if (root == NULL)
        return;
    BSTvisit_datesIntervalR(root->l, date1, date2, max, min, flag1_p, flag2_p);

    current = root->quot;
    current_date = quotation_DATEget(current.dataOra);
    if (current_date >= date1 && current_date <= date2) {
        if (QUOTATIONcompute(current) > QUOTATIONcompute(*max)) {
            *max = current;
            (*flag1_p)++;
        }
        if (QUOTATIONcompute(current) < QUOTATIONcompute(*min)) {
            *min = current;
            (*flag2_p)++;
        }
    }

    BSTvisit_datesIntervalR(root->r, date1, date2, max, min, flag1_p, flag2_p);
}


void BSTvisit_datesInterval(BST bst, dateTime dateTime1, dateTime dateTime2, int all) {
    int date1, date2, flag1=0, flag2=0;
    quotation max, min;

    max.vn = 1; max.n = INT_MAX;
    min.vn = INT_MIN; min.n = -1;

    if (all) {
        date1 = 0;
        date2 = INT_MAX;
    }
    else {
        date1 = quotation_DATEget(dateTime1);
        date2 = quotation_DATEget(dateTime2);
    }

    BSTvisit_datesIntervalR(bst->root, date1, date2, &max, &min, &flag1, &flag2);

    if (flag1 == 0 && flag2 == 0)
        printf("Non ci sono quotazioni tra queste date\n");
    else {
        printf("Quotazione massima:\n\t");
        QUOTATIONstore(max);
        printf("Quotazione minima:\n\t");
        QUOTATIONstore(min);
    }
}


static link partR(link h, int r) {
    int t = h->l->N;
    if (t > r) {
        h->l = partR(h->l, r);
        h = rotR(h);
    }
    if (t < r) {
        h->r = partR(h->r, r-t-1);
        h = rotL(h);
    }
    return h;
}


static void BSTminHeightR(link root, int *min_p, int *len_p) {
    if (root == NULL) {
        (*len_p)--;
        if (*len_p < *min_p)
            *min_p = *len_p;
        return;
    }

    (*len_p)++;
    BSTminHeightR(root->l, min_p, len_p);

    (*len_p)++;
    BSTminHeightR(root->r, min_p, len_p);
    (*len_p)--;
}


int BSTminHeight(BST bst) {
    int min = INT_MAX, len=0;
    BSTminHeightR(bst->root, &min, &len);
    return min;
}


static void BSTmaxHeightR(link root, int *max_p, int *len_p) {
    if (root == NULL) {
        (*len_p)--;
        if (*len_p > *max_p)
            *max_p = *len_p;
        return;
    }

    (*len_p)++;
    BSTmaxHeightR(root->l, max_p, len_p);

    (*len_p)++;
    BSTmaxHeightR(root->r, max_p, len_p);
    (*len_p)--;
}


int BSTmaxHeight(BST bst) {
    int max = INT_MIN, len=0;
    BSTmaxHeightR(bst->root, &max, &len);
    return max;
}


static link balanceR(link h, link z) {
    int r;
    if (h == z)
        return z;
    r = (h->N+1)/2-1;
    h = partR(h, r);
    h->l = balanceR(h->l, z);
    h->r = balanceR(h->r, z);
    return h;
}


void BSTbalance(BST bst) {
    bst->root = balanceR(bst->root, bst->z);
}


void print2DBST(link root, int space) {
    if (root == NULL)
        return;

    space += 10;

    print2DBST(root->r, space);

    printf("\n");
    for (int i = 10; i < space; i++)
        printf(" ");
    DATEstore(root->quot.dataOra);
    printf("\n");

    print2DBST(root->l, space);
}

/* wrapper per print2DUtil() */
void print2DBST_wrapper(BST bst) {
    print2DBST(bst->root, 0);
}


static void checkErroreAllocazione(void *p) {
    if (p == NULL) {
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
}