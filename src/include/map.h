#ifndef P6K14_INTERNAL_HASHMAP_H
#define P6K14_INTERNAL_HASHMAP_H

#include <stdbool.h>
#include <stddef.h>
#include "internal.h"

typedef struct stHashMap HashMap;

P6K14_INTERNAL HashMap *MakeHashMap(void);
P6K14_INTERNAL HashMap *MakeHashMap2(size_t nbucket);
P6K14_INTERNAL void FreeHashMap(HashMap *hm, void (*dtor)(void *));
P6K14_INTERNAL void *HashMapGet(HashMap *hm, const char *k);
P6K14_INTERNAL bool HashMapSet(HashMap *hm, const char *k, void *v);

typedef struct stVecMap VecMap;
P6K14_INTERNAL VecMap *MakeVecMap(size_t nslot);
P6K14_INTERNAL void FreeVecMap(VecMap *vm, void (*dtor)(void *));
P6K14_INTERNAL void *VecMapGet(VecMap *vm, const char *k);
P6K14_INTERNAL bool VecMapSet(VecMap *vm, const char *k, void *v);

#endif /* P6K14_INTERNAL_HASHMAP_H */
