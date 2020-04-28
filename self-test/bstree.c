#include "bstree.h"
#include <assert.h>

typedef struct node node;

struct node
{
    int value;
    node *leftNode;
    node *rightNode;
};

typedef struct bstree
{
    node *root;
    int nodeCounter;
} bstree;

static void free_nodes(node *node);
static node *node_create(int value);
static int bstree_minimum_helper(const bstree *tree, const node *currentNode);
static int bstree_maximum_helper(const bstree *tree, const node *currentNode);
static void node_print(FILE *out, node *node);
static node *node_delete(bstree *t, node *node, int value);

bstree *bstree_create()
{
    bstree *tree = malloc(sizeof(bstree));
    tree->nodeCounter = 0;
    tree->root = NULL;

    return tree;
}

void bstree_destroy(bstree *t)
{
    if (t != NULL)
    {
        free_nodes(t->root);
        free(t);
    }
}

static void free_nodes(node *node)
{
    if (node == NULL)
        return;
    free_nodes(node->leftNode);
    free_nodes(node->rightNode);

    free(node);
}

void bstree_insert(bstree *t, int d)
{
    if (bstree_contains(t, d))
        return;
    if (t->root == NULL)
    {
        t->root = node_create(d);
        t->nodeCounter = 1;
    }
    else
    {
        node *currentNode = t->root;
        bool r = true;

        while (r)
        {
            if (currentNode->value == d)
                r = false;
            else if (currentNode->value > d)
            {
                if (currentNode->leftNode == NULL)
                {
                    currentNode->leftNode = node_create(d);
                    t->nodeCounter++;
                    r = false;
                }
                else
                {
                    currentNode = currentNode->leftNode;
                }
            }
            else
            {
                if (currentNode->rightNode == NULL)
                {
                    currentNode->rightNode = node_create(d);
                    t->nodeCounter++;
                    r = false;
                }
                else
                {
                    currentNode = currentNode->rightNode;
                }
            }
        }
    }
}

static node *node_create(int value)
{
    node *n = malloc(sizeof(node));
    n->value = value;
    n->leftNode = NULL;
    n->rightNode = NULL;

    return n;
}

void bstree_remove(bstree *t, int d)
{
    if (bstree_contains(t, d))
    {
        node_delete(t, t->root, d);
    }
}

static node *node_delete(bstree *t, node *removeNode, int value)
{
    if (removeNode->value > value)
    {
        if (removeNode->leftNode != NULL)
        {
            return node_delete(t, removeNode->leftNode, value);
        }
    }
    else if (removeNode->value < value)
    {
        if (removeNode->rightNode != NULL)
        {
            return node_delete(t, removeNode->rightNode, value);
        }
    }
    else
    {
        if (removeNode->leftNode == NULL && removeNode->rightNode == NULL)
        {
            free(removeNode);
            t->nodeCounter--;
            return NULL;
        }
        else if (removeNode->leftNode != NULL && removeNode->rightNode == NULL)
        {
            node *newNode = removeNode->leftNode;
            free(removeNode);
            t->nodeCounter--;
            return newNode;
        }
        else if (removeNode->rightNode != NULL && removeNode->leftNode == NULL)
        {
            node *newNode = removeNode->rightNode;
            free(removeNode);
            t->nodeCounter--;
            return newNode;
        }
        else
        {
            int minimumValue = bstree_minimum_helper(t, removeNode);
            removeNode->value = minimumValue;
            node_delete(t, removeNode->rightNode, minimumValue);
            node_delete(t, removeNode->leftNode, minimumValue);
        }
    }
    return removeNode;
}

int bstree_minimum(const bstree *t)
{
    return bstree_minimum_helper(t, t->root);
}

static int bstree_minimum_helper(const bstree *tree, const node *currentNode)
{
    if (currentNode == NULL)
        return -1;
    else if (currentNode->leftNode == NULL)
        return currentNode->value;
    else
        return bstree_minimum_helper(tree, currentNode->leftNode);
}

int bstree_maximum(const bstree *t)
{
    return bstree_maximum_helper(t, t->root);
}

static int bstree_maximum_helper(const bstree *tree, const node *currentNode)
{
    if (currentNode == NULL)
        return -1;
    else if (currentNode->rightNode == NULL)
        return currentNode->value;
    else
        return bstree_minimum_helper(tree, currentNode->rightNode);
}

bool bstree_contains(const bstree *t, int d)
{
    if (t->nodeCounter > 0)
    {
        node *currentNode = t->root;
        while (true)
        {
            if (currentNode == NULL)
                return false;
            if (currentNode->value == d)
            {
                return true;
            }
            else if (currentNode->value < d)
            {
                currentNode = currentNode->rightNode;
            }
            else if (currentNode->value > d)
            {
                currentNode = currentNode->leftNode;
            }
        }
    }
    return false;
}

size_t bstree_size(const bstree *t)
{
    return t->nodeCounter;
}

void bstree_print(const bstree *t, FILE *out)
{
    node_print(out, t->root);
    printf(" : %ld\n", bstree_size(t));
}

static void node_print(FILE *out, node *node)
{
    if (node == NULL)
    {
        fprintf(out, "(NULL)");
        return;
    }

    fprintf(out, "(");

    if (node->leftNode != NULL)
        node_print(out, node->leftNode);

    fprintf(out, "%d", node->value);

    if (node->rightNode != NULL)
        node_print(out, node->rightNode);

    fprintf(out, ")");
}