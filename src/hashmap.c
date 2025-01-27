#include "map.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "internal.h"

typedef struct stNode {
    struct stNode *next;
    void *v;
    char k[];
} Node;

typedef struct stListMap {
    Node *list;
    Node *tail;
} ListMap;

typedef struct stHashMap {
    size_t nbucket;
    Node* buckets[];
} HashMap;

static uint64_t BKDRHash(char const *str);
static void *ReadChain(Node *chain, char const *k);
static void FreeChain(Node *chain, void (*dtor)(void *));

P6K14_INTERNAL HashMap *MakeHashMap(void) {
    return MakeHashMap2(64);
}

P6K14_INTERNAL HashMap *MakeHashMap2(size_t nbucket) {
    HashMap *ret = malloc(sizeof(HashMap) + nbucket * sizeof(Node*));
    memset(ret, 0, sizeof(HashMap) + nbucket * sizeof(Node*));
    ret->nbucket = nbucket;
    return ret;
}

P6K14_INTERNAL void FreeHashMap(HashMap *hm, void (*dtor)(void *)) {
    for (size_t i = 0; i < hm->nbucket; i++) {
        FreeChain(hm->buckets[i], dtor);
    }
    free(hm);
}

P6K14_INTERNAL void *HashMapGet(HashMap *hm, const char *k) {
    Node *chain = hm->buckets[BKDRHash(k) % hm->nbucket];
    return ReadChain(chain, k);
}

P6K14_INTERNAL bool HashMapSet(HashMap *hm, const char *k, void *v) {
    size_t idx = BKDRHash(k) % hm->nbucket;
    Node *chain = hm->buckets[idx];
    if (chain == NULL) {
        Node *new_node = malloc(sizeof(Node) + strlen(k) + 1);
        new_node->next = NULL;
        new_node->v = v;
        strcpy(new_node->k, k);
        hm->buckets[idx] = new_node;
        return true;
    }

    if (!strcmp(chain->k, k)) {
        return false;
    }

    while (chain->next) {
        if (!strcmp(chain->k, k)) {
            return false;
        }
        chain = chain->next;
    }

    Node *new_node = malloc(sizeof(Node) + strlen(k) + 1);
    new_node->next = NULL;
    new_node->v = v;
    strcpy(new_node->k, k);
    chain->next = new_node;
    return true;
}

static void* ReadChain(Node *chain, char const *k) {
    while (chain) {
        if (!strcmp(chain->k, k)) {
            return chain->v;
        }
        chain = chain->next;
    }
    return NULL;
}

static void FreeChain(Node *chain, void (*dtor)(void *)) {
    while (chain) {
        Node *next = chain->next;
        if (dtor) {
            dtor(chain->v);
        }
        free(chain);
        chain = next;
    }
}

static uint64_t BKDRHash(char const* str) {
    uint64_t hash = 0;
    char ch;
    while ((ch = *str++)) {
        hash = hash * 131 + ch;
    }
    return hash;
}
