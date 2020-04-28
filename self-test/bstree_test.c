#include "bstree.h"

#include <assert.h>

int main()
{
    bstree *ta = bstree_create();

    assert(bstree_size(ta) == 0);
    assert(bstree_contains(ta, 4) == false);
    bstree_print(ta, stdout);

    bstree_insert(ta, 4);
    bstree_insert(ta, 7);
    bstree_insert(ta, 3);

    assert(bstree_size(ta) == 3);
    assert(bstree_contains(ta, 4) == true);
    assert(bstree_minimum(ta) == 3);
    assert(bstree_maximum(ta) == 7);
    bstree_print(ta, stdout);

    bstree_remove(ta, 4);
    bstree_print(ta, stdout);

    assert(bstree_size(ta) == 2);
    assert(bstree_contains(ta, 4) == false);
    assert(bstree_minimum(ta) == 3);
    assert(bstree_maximum(ta) == 7);
    bstree_print(ta, stdout);

    bstree_insert(ta, 2);
    bstree_insert(ta, -3);
    bstree_insert(ta, 6);
    bstree_insert(ta, 9);

    assert(bstree_size(ta) == 6);
    bstree_print(ta, stdout);

    bstree_remove(ta, 6);

    assert(bstree_size(ta) == 5);
    assert(bstree_contains(ta, 6) == false);
    bstree_print(ta, stdout);

    bstree_insert(ta, 5);
    bstree_insert(ta, 5);

    assert(bstree_size(ta) == 6);
    assert(bstree_contains(ta, 5) == true);
    bstree_print(ta, stdout);

    bstree_insert(ta, 6);
    bstree_insert(ta, 4);
    bstree_remove(ta, 7);
    bstree_remove(ta, 7);

    assert(bstree_size(ta) == 7);
    assert(bstree_contains(ta, 7) == false);
    bstree_print(ta, stdout);

    bstree_destroy(ta);
}