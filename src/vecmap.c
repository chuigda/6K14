#include "map.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "internal.h"

typedef struct stNode {
    void *v;
    char k[];
} Node;

typedef struct stVecMap {
    size_t nslot;
    size_t size;
    Node *slots[];
} VecMap;

P6K14_INTERNAL VecMap *MakeVecMap(size_t nslot) {
    VecMap *ret = malloc(sizeof(VecMap) + nslot * sizeof(Node*));
    ret->nslot = nslot;
    ret->size = 0;
    return ret;
}

P6K14_INTERNAL void FreeVecMap(VecMap *vm, void (*dtor)(void *)) {
    for (size_t i = 0; i < vm->size; i++) {
        if (dtor) {
            dtor(vm->slots[i]->v);
        }
        free(vm->slots[i]);
    }
    free(vm);
}

P6K14_INTERNAL void *VecMapGet(VecMap *vm, const char *k) {
    for (size_t i = 0; i < vm->size; i++) {
        if (!strcmp(vm->slots[i]->k, k)) {
            return vm->slots[i]->v;
        }
    }
    return NULL;
}

P6K14_INTERNAL bool VecMapSet(VecMap *vm, const char *k, void *v) {
    assert(vm->size < vm->nslot);

    for (size_t i = 0; i < vm->size; i++) {
        if (!strcmp(vm->slots[i]->k, k)) {
            return false;
        }
    }

    Node *new_node = malloc(sizeof(Node) + strlen(k) + 1);
    new_node->v = v;
    strcpy(new_node->k, k);
    vm->slots[vm->size++] = new_node;
    return true;
}
