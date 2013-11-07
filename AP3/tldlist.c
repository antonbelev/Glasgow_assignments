/*
* Author: Anton Belev
* ID: 1103816b
* AP3 Exercise 1
*/

#include "tldlist.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct tldlist{
    Date *begin;
    Date *end;
    TLDNode *root;
    long size;
    long totalTLDs;
};

struct tldnode{
    TLDNode *parent;
    TLDNode *left;
    TLDNode *right;
    long count;
    char *tld;
    long height;
};

struct tlditerator{
    TLDNode **inorder;
    long next;
    long size;
};

static int isEmpty(TLDList *list)
{
    return list->root == NULL ? 1 : 0;
}

static int isLeftChild(TLDNode *node)
{
   if (node->parent == NULL)
        return 0;
   return node == (node->parent->left) ? 1 : 0;
}

static int hasLeft(TLDNode *node)
{
   return node->left != NULL ? 1 : 0;
}

static int hasRight(TLDNode *node)
{
   return node->right != NULL ? 1 : 0;
}

static long getLeftHeight(TLDNode *x)
{
    if (x == NULL)
        return 0;
    if (x->left == NULL)
        return 0;
    return x->left->height;
}

static long getRightHeight(TLDNode *x)
{
    if (x == NULL)
        return 0;
    if (x->right == NULL)
        return 0;
    return x->right->height;
}

static long getLeftRightHeight(TLDNode *x)
{
    if (x == NULL)
        return 0;
    if (x->left == NULL)
        return 0;
    if (x->left->right == NULL)
        return 0;
    return x->left->right->height;
}

static long getLeftLeftHeight(TLDNode *x)
{
    if (x == NULL)
        return 0;
    if (x->left == NULL)
        return 0;
    if (x->left->left == NULL)
        return 0;
    return x->left->left->height;
}

static long getRightRightHeight(TLDNode *x)
{
    if (x == NULL)
        return 0;
    if (x->right == NULL)
        return 0;
    if (x->right->right == NULL)
        return 0;
    return x->right->right->height;
}

static long getRightLeftHeight(TLDNode *x)
{
    if (x == NULL)
        return 0;
    if (x->right == NULL)
        return 0;
    if (x->right->left == NULL)
        return 0;
    return x->right->left->height;
}

static void updateParent(TLDNode *parent, TLDNode *node)
{
    if (node == NULL)
        return;
    node->parent = parent;
}

TLDNode *tldnode_create(TLDNode *parent, char *tld, long height)
{
    TLDNode *node;
    node = (TLDNode *)malloc(sizeof(TLDNode));
    if (node == NULL)
        return NULL;
    node->parent = parent;
    node->left = NULL;
    node->right = NULL;
    node->count = 1;
    node->tld = tld;
    node->height = height;
    return node;
}

static long math_max(long x1, long x2)
{
    if (x1 >= x2)
        return x1;
    else
        return x2;
}

static void updateHeight(TLDNode *node)
{
    if (node != NULL)
    {
        if (node->left != NULL && node->right != NULL)
            node->height = math_max(node->left->height, node->right->height) + 1;
        else if (node->left != NULL)
            node->height = math_max(node->left->height, 0) + 1;
        else if (node->right != NULL)
            node->height = math_max(0, node->right->height) + 1;
        else
            node->height = 1;
        updateHeight(node->parent);
    }
}

static int isImbalanced(TLDNode *node)
{
    if (node == NULL)
        return 0;
    if (node->left != NULL && node->right != NULL)
    {
        long diff = (node->left)->height - (node->right)->height;
        if ( diff < -1 || diff > 1)
            return 1;
        else
            return 0;
    }
    else
    {
        if (node->left != NULL && node->left->height > 1)
            return 1;
        else if (node->right != NULL && node->right->height > 1)
            return 1;
        else
            return 0;
    }
}

static TLDNode *findImbalanced(TLDNode *x)
{
    if (x == NULL)
        return NULL;
    else if (isImbalanced(x))
        return x;
    else
        return findImbalanced(x->parent);
}

static int case1(TLDNode *x)
{
    return (getLeftHeight(x) > getRightHeight(x))
            && (getLeftLeftHeight(x) > getLeftRightHeight(x));
}

static int case2(TLDNode *x)
{
    return (getLeftHeight(x) > getRightHeight(x))
            && ( getLeftRightHeight(x) > getLeftLeftHeight(x));
}

static int case3(TLDNode *x)
{
    return (getLeftHeight(x) < getRightHeight(x))
            && ( getRightLeftHeight(x) > getRightRightHeight(x));
}

static int case4(TLDNode *x)
{
    return (getLeftHeight(x) < getRightHeight(x))
            && ( getRightRightHeight(x) > getRightLeftHeight(x));
}

static void LLRodation(TLDNode *x, TLDList *tld)
{
    TLDNode *k2 = x;
    TLDNode *k1 = k2->left;
    TLDNode *p = k2->parent;
    if (p == NULL)
    {
        tld->root = k1;
        k1->parent = NULL;
    }
    else if (isLeftChild(k2))
        p->left = k1;
    else
        p->right = k1;

    k2->left = k1->right;
    updateParent(k2, k1->right);
    k2->parent = k1;
    k1->parent = p;
    k1->right = k2;
    updateHeight(k2);
 }

 static void RRRotation(TLDNode *x, TLDList *tld)
{
    TLDNode *k1 = x;
    TLDNode *k2 = x->right;
    TLDNode *p = k1->parent;
    if (p == NULL)
    {
        tld->root = k2;
        k2->parent = NULL;
    }
    else if (isLeftChild(k1))
        p->left = k2;
    else
        p->right = k2;

    k1->right = k2->left;
    updateParent(k1, k2->left);
    k1->parent = k2;
    k2->parent = p;
    k2->left = k1;
    updateHeight(k1);
 }

 static void doubleRotate(TLDNode *k1, TLDNode *k2, TLDNode *k3, TLDNode *p, int isLeftChild, TLDList *tld)
{
    if (p == NULL)
    {
        tld->root = k2;
        k2->parent = NULL;
    }
    else if (isLeftChild)
    {
        p->left = k2;
        k2->parent = p;
    }
    else
    {
        p->right = k2;
        k2->parent = p;
    }
    k3->left = k2->right;
    updateParent(k3, k2->right);
    k1->right = k2->left;
    updateParent(k1, k2->left);
    k2->left = k1;
    k2->right = k3;
    k1->parent = k2;
    k3->parent = k2;
    updateHeight(k1);
    updateHeight(k3);
}

static void LRRotation(TLDNode *x, TLDList *tld)
{
    TLDNode *k1, *k2, *k3, *p;
    k3 = x;
    k1 = k3->left;
    k2 = k1->right;
    p = k3->parent;
    doubleRotate(k1,k2,k3,p, p != NULL && isLeftChild(k3), tld);
}

static void RLRotation(TLDNode *x, TLDList *tld)
{
    TLDNode *k1, *k2, *k3, *p;
    k1 = x;
    k3 = k1->right;
    k2 = k3->left;
    p = k1->parent;
    doubleRotate(k1,k2,k3,p, p != NULL && isLeftChild(k1), tld);
}

static void restoreBalance(TLDNode *node, TLDList *tld)
{
    TLDNode *x = findImbalanced(node->parent);
    if (x != NULL)
    {
        if (case1(x))
            LLRodation(x, tld);
        else if (case2(x))
            LRRotation(x, tld);
        else if (case3(x))
            RLRotation(x, tld);
        else if (case4(x))
            RRRotation(x, tld);
    }
}

TLDList *tldlist_create(Date *begin, Date *end)
{
    TLDList *tldlist;
    tldlist = (TLDList *)malloc(sizeof(TLDList));
    if (tldlist == NULL)
        return NULL;
    tldlist->begin = begin;
    tldlist->end = end;
    tldlist->root = NULL;
    tldlist->size = 0;
    tldlist->totalTLDs = 0;
    return tldlist;
}

static void iterate_nodes_destroy(TLDNode *node)
{
    if (node == NULL)
        return;
    iterate_nodes_destroy(node->left);
    iterate_nodes_destroy(node->right);
    free(node->tld);
    free(node);
}

void tldlist_destroy(TLDList *tld)
{
    iterate_nodes_destroy(tld->root);
    free(tld);
}

static int tldlist_insert(char *tldStr, TLDNode *node, TLDList *tld)
{
    int comparison = strcmp(tldStr, node->tld);
    if (comparison == 0)
    {
        node->count++;
        free(tldStr);
        return 2; //tld already is in the tree
    }
    else if (comparison < 0 && node->left == NULL)
    {
        node->left = tldnode_create(node, tldStr, 1);
        if (node->left == NULL)
            return 0;
        updateHeight(node->left);
		restoreBalance(node->left, tld);
        return 1;//TODO height
    }
    else if (comparison < 0 && node->left != NULL)
        return tldlist_insert(tldStr, node->left, tld);
    else if (comparison > 0 && node->right == NULL)
    {
        node->right = tldnode_create(node, tldStr, 1);
        if (node->right == NULL)
            return 0;
        updateHeight(node->right);
		restoreBalance(node->right, tld);
        return 1;
    }
    else if (comparison > 0 && node->right != NULL)
        return tldlist_insert(tldStr, node->right, tld);
    return 0;
}

int tldlist_add(TLDList *tld, char *hostname, Date *d)
{
    if (date_compare(tld->begin, d) > 0 || date_compare(tld->end, d) < 0 )
        return 0;
    char *tldStr, *cur;
    cur = hostname;
    while ( *cur != '\0')
    {
        if ( *cur == '.')
        {
            tldStr = cur;
        }
        cur++;
    }
    tldStr++;
    char *tldValue = (char *)malloc(sizeof(char)*4);
    strcpy(tldValue, tldStr);
    if (isEmpty(tld))
    {
        tld->root = tldnode_create(NULL, tldValue, 1);
        if (tld->root != NULL)
        {
            tld->size++;
            tld->totalTLDs++;
            return 1;
        }
        else
            return 0;
    }
    else
    {
        int result = tldlist_insert(tldValue, tld->root, tld);
        if ( result != 0 )
        {
            if (result != 2) // if tld already exists, you don't need to increment the size of the tree.
                tld->size++;
            tld->totalTLDs++;
            return 1;
        }
        else
            return 0;
    }
    return 0;
}

long tldlist_count(TLDList *tld)
{
    return tld->totalTLDs;
}

void inorder_traversal(TLDNode *inorder[], long *curIndex, long size, TLDNode *node)
{
    if (hasLeft(node))
    {
        inorder_traversal(inorder, curIndex, size, node->left);
    }
    inorder[*curIndex] = node;
    (*curIndex)++;
    if (hasRight(node))
    {
        inorder_traversal(inorder, curIndex, size, node->right);
    }
}

TLDIterator *tldlist_iter_create(TLDList *tld)
{
    TLDIterator *iter;
    TLDNode **inorder = (TLDNode **)malloc(sizeof(TLDNode *)*tld->size);
    iter = (TLDIterator *)malloc(sizeof(TLDIterator));
    if (iter == NULL)
        return NULL;
    long *currentIndex = (long *)malloc(sizeof(long));
    *currentIndex = 0;
    inorder_traversal(inorder, currentIndex, tld->size, tld->root);
    free(currentIndex);
    iter->inorder = inorder;
    iter->size = tld->size;
    iter->next = 0;
    return iter;
}
TLDNode *tldlist_iter_next(TLDIterator *iter)
{
    if (iter->next >= iter->size)
        return NULL;
    return (iter->inorder)[iter->next++];
}
void tldlist_iter_destroy(TLDIterator *iter)
{
    free(iter->inorder);
    free(iter);
}
char *tldnode_tldname(TLDNode *node)
{
    return node->tld;
}
long tldnode_count(TLDNode *node)
{
    return node->count;
}
