#ifndef P6K14_INTERNAL_HASHMAP_H
#define P6K14_INTERNAL_HASHMAP_H

typedef struct stHashMap HashMap;

HashMap *MakeHashMap(void);
void FreeHashMap(HashMap *hm);
void *HashMapGet(HashMap *hm, const char *k);
void HashMapSet(HashMap *hm, const char *k, void *v);

typedef struct stListMap ListMap;
ListMap *MakeListMap(void);
void FreeListMap(ListMap *lm);
void *ListMapGet(ListMap *lm, const char *k);
void ListMapSet(ListMap *lm, const char *k, void *v);

#endif /* P6K14_INTERNAL_HASHMAP_H */
