#include "GenList.h"

/* GenList internal ----------- */

#define INITIAL_CAPACITY   32
#define LIST_AS_LIS        _GENLIST_ *lis = (_GENLIST_*)list
#define MUST_NOT_NULL      if (!list) { last_err = GENLIST_ERR_UNEXPECTED_NULL; return false; }

typedef struct _GENLIST_ {
    size_t item_size;
    size_t used;
    size_t capacity;
    void **data;
    bool   do_memcpy;
} _GENLIST_;

size_t a;

void ReallocAsNeeded(_GENLIST_ *lis) {
    if (lis->used >= lis->capacity) {
        lis->capacity += INITIAL_CAPACITY;
        lis->data = (void**)realloc(lis->data, lis->capacity * lis->item_size);
    }
}

/* ----------- GenList internal */

int last_err = 0;

int GenList_LastErr() { return last_err; }

bool GenList_Create(GENLIST *list, size_t item_size, bool do_memcpy) {
    if (!list || !item_size) {
        last_err = GENLIST_ERR_UNEXPECTED_NULL;
        return false;
    }
    _GENLIST_ *lis = (_GENLIST_*)malloc(sizeof(_GENLIST_));
    lis->capacity = 0;
    lis->used = 0;
    lis->data = NULL;
    lis->item_size = item_size;
    lis->do_memcpy = do_memcpy;
    ReallocAsNeeded(lis);
    *list = lis;
    return true;
}

bool GenList_Destroy(GENLIST list) {
    if (GenList_Clear(list)) {
        free(list);
        return true;
    }
    return false;
}

bool GenList_GetInfo(GENLIST list, size_t *count, size_t *item_size, bool *do_memcpy) {
    MUST_NOT_NULL; LIST_AS_LIS;
    if (count) { *count = lis->used; }
    if (item_size) { *item_size = lis->item_size; }
    if (do_memcpy) { *do_memcpy = lis->do_memcpy; }
    return true;
}

bool GenList_Add(GENLIST list, void *item) {
    MUST_NOT_NULL; LIST_AS_LIS;
    ReallocAsNeeded(lis);
    size_t i = lis->used;
    if (lis->do_memcpy) {
        lis->data[i] = (void*)malloc(lis->item_size);
        memcpy(lis->data[i], item, lis->item_size);
    } else {
        lis->data[i] = item;
    }
    ++(lis->used);
    return true;
}

bool GenList_AddAt(GENLIST list, size_t i, void *item) {
    MUST_NOT_NULL; LIST_AS_LIS;
    // When Count==0, it's OK to AddAt(..., 0, &foo_var);
    size_t max_i = lis->used - 1;
    if (i > max_i + 1) {
        last_err = GENLIST_ERR_OUT_OF_RANGE;
        return false;
    }
    ReallocAsNeeded(lis);
    // Let items = [a][b][c][d][ ] , item = [*] , i = 2
    // First, [a][b][c][c][d]
    //  then, [a][b][*][c][d]
    for (a = max_i;; --a) /* from max_i to i */ {
        lis->data[a+1] = lis->data[a];
        if (a == i) { break; }
    }
    if (lis->do_memcpy) {
        lis->data[a] = (void*)malloc(lis->item_size);
        memcpy(lis->data[a], item, lis->item_size);
    } else {
        lis->data[a] = item;
    }
    ++(lis->used);
    return true;
}

bool GenList_GetAt(GENLIST list, size_t i, void **item) {
    if (!list || !item) {
        last_err = GENLIST_ERR_UNEXPECTED_NULL;
        return false;
    }
    LIST_AS_LIS;
    size_t max_i = lis->used - 1;
    if (i > max_i) {
        last_err = GENLIST_ERR_OUT_OF_RANGE;
        return false;
    }
    if (lis->do_memcpy) {
        memcpy(item, lis->data[i], lis->item_size);
    } else {
        *item = lis->data[i];
    }
    return true;
}

bool GenList_Clear(GENLIST list) {
    MUST_NOT_NULL; LIST_AS_LIS;
    if (lis->do_memcpy) {
        for (a = 0; a < lis->used; ++a) { free(lis->data[a]); }
    }
    lis->used = 0;
    return true;
}

bool GenList_RemoveAt(GENLIST list, size_t i) {
    MUST_NOT_NULL; LIST_AS_LIS;
    size_t max_i = lis->used - 1;
    if (i > max_i) {
        last_err = GENLIST_ERR_OUT_OF_RANGE;
        return false;
    }
    if (lis->do_memcpy) { free(lis->data[i]); }
    // let items = [a][b][c][d], i = 2
    // let it be [a][b][d][ ]
    for (a = i; a < max_i; ++a) {
        lis->data[a] = lis->data[a+1];
    }
    --(lis->used);
    return true;
}

bool GenList_ForEach(GENLIST list, GENLIST_FOREACH_HANDLER fn) {
    MUST_NOT_NULL; LIST_AS_LIS;
    for (a = 0; a < lis->used; ++a) {
        fn(a, lis->data[a]);
    }
    return true;
}
