#include "tldlist.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct date {
    char *year;
    char *month;
    char *day;
};

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
    int height;
};

//TODO
struct tlditerator{
    TLDNode **inorder;
    long next;
    long size;
};

static int isEmpty(TLDList *list)
{
    return list->root == NULL ? 1 : 0;
}

TLDNode *tldnode_create(TLDNode *parent, char *tld, int height)
{
    //TODO consider passing parent and tld only
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

//TLDNode functuons
static int degree(TLDNode *node)
{
    int degree = 0;
    if (node->left != NULL)
        degree++;
    if (node->right != NULL)
        degree++;
    return degree;
}

static int isRoot(TLDNode *node)
{
   return node->parent == NULL ? 1 : 0;
}

static int isLeaf(TLDNode *node)
{
   return degree(node) == 0 ? 1 : 0;
}

static int isInternal(TLDNode *node)
{
   return degree(node) == 2 ? 1 : 0;
}

static int isLeftChild(TLDNode *node)
{
   return &node == &(node->parent->left) ? 1 : 0;
}

static int isRightChild(TLDNode *node)
{
   return &node == &(node->parent->right) ? 1 : 0;
}

static int hasLeft(TLDNode *node)
{
   return node->left != NULL ? 1 : 0;
}

static int hasRight(TLDNode *node)
{
   return node->right != NULL ? 1 : 0;
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

void tldlist_destroy(TLDList *tld)
{
    //TODO iterate through the TLDNodes
    free(tld);
}

static int tldlist_insert(char *tldStr, TLDNode *node)
{
    int comparison = strcmp(tldStr, node->tld);
    if (comparison == 0)
    {
        node->count++;
        return 2; //tld already is in the tree
    }
    else if (comparison < 0 && node->left == NULL)
    {
        node->left = tldnode_create(node, tldStr, 0);
        if (node->left == NULL)
            return 0;
        return 1;//TODO height
    }
    else if (comparison < 0 && node->left != NULL)
        return tldlist_insert(tldStr, node->left);
    else if (comparison > 0 && node->right == NULL)
    {
        node->right = tldnode_create(node, tldStr, 0);
        if (node->right == NULL)
            return 0;
        return 1;//TODO height
    }
    else if (comparison > 0 && node->right != NULL)
        return tldlist_insert(tldStr, node->right);

    return 0;
}

int tldlist_add(TLDList *tld, char *hostname, Date *d)
{
    /*printf("Date D is: %s/%s/%s \n", (*d).day, (*d).month, (*d).year);
    printf("Date Begin is: %s/%s/%s \n", tld->begin->day, tld->begin->month, tld->begin->year);
    printf("Date End is: %s/%s/%s \n", tld->end->day, tld->end->month, tld->end->year);
    printf("Compare begin with date: %d\n", date_compare(tld->begin, d));
    printf("Compare end with date: %d\n", date_compare(tld->end, d));*/
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
    tldStr++; //TLD
    char *tldValue = (char *)malloc(sizeof(char)*4);
    strcpy(tldValue, tldStr);
    //printf("The TLD is: %s\n", tldValue);
    //printf("TLD size before: %d\n", tld->size);
    if (isEmpty(tld))
    {
        tld->root = tldnode_create(NULL, tldValue, 0);
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
        int result = tldlist_insert(tldValue, tld->root);
        if ( result != 0 )
        {
            if (result != 2) // if tld already exists, you don't need to increment the size of the tree.
                tld->size++;
            tld->totalTLDs++;
            //printf("TLD total: %d\n\n", tld->totalTLDs);
            return 1;
        }
        else
            return 0;
    }
    //TODO remove default return
    return 0;
}

long tldlist_count(TLDList *tld)
{
    return tld->totalTLDs;
}

void inorder_traversal(TLDNode *inorder[], long *curIndex, long size, TLDNode *node)
{
    //printf("CurIndex - top %ld\n", *curIndex);
    if (hasLeft(node))
    {
        //printf("Node %s has left = %ld, curIndex=%d\n", node->tld, hasLeft(node), *curIndex);
        inorder_traversal(inorder, curIndex, size, node->left);
    }
    //printf("Node %s\n", node->tld);
    inorder[*curIndex] = node;
    (*curIndex)++;
    //printf("CurIndex - middle %ld\n", *curIndex);
    //printf("Current index of traversal: %d\n For Node: %s", curIndex, node->tld);
    if (hasRight(node))
    {
        //printf("Node %s has right = %ld curIndex=%d\n", node->tld, hasRight(node), *curIndex);
        inorder_traversal(inorder, curIndex, size, node->right);
    }
    //printf("CurIndex - bottom %ld\n\n", *curIndex);

}

TLDIterator *tldlist_iter_create(TLDList *tld)
{
    TLDIterator *iter;
    TLDNode **inorder = (TLDNode **)malloc(sizeof(TLDNode *)*tld->size);
    iter = (TLDIterator *)malloc(sizeof(TLDIterator));
    if (iter == NULL)
        return NULL;
    //inorder = malloc(sizeof(TLDNode *)*(tld->size));
    long i;
    /*for(i = 0; i < tld->size; i++)
    {
        inorder[i] = (TLDNode *)malloc(sizeof(TLDNode *));
        //printf("Malloc successful [%d], %d\n", i, inorder[i] == NULL);
    }*/

    //if (inorder == NULL || tld->size)
    //    return NULL;
    long *currentIndex = (long *)malloc(sizeof(long));
    *currentIndex = 0;
    inorder_traversal(inorder, currentIndex, tld->size, tld->root);
    //printf("size is : %ld \n", tld->size);
    for(i = 0; i < tld->size; i++)
    {
        //printf("i=%d\n", i);
        printf("Node[%d] = %s\n", i, inorder[i]->tld);
    }
    iter->inorder = inorder; // create inorder traversal of the tld list and save the pointers in this array
    iter->size = tld->size;
    iter->next = 0; //TODO check if correct
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
    //TODO
}
char *tldnode_tldname(TLDNode *node)
{
    return node->tld;
}
long tldnode_count(TLDNode *node)
{
    return node->count;
}
