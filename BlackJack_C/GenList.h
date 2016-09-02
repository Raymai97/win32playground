// GenList v2.0 written by Raymai97 on July 2016, C99 mode required

#ifndef _GENLIST_H
#define _GENLIST_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef void* GENLIST;
typedef void (*GENLIST_FOREACH_HANDLER)(int, void*);

bool GenList_Create(GENLIST*, size_t item_size, bool do_memcpy);
bool GenList_Destroy(GENLIST);

// it's OK to pass NULL like this:
// ...GetInfo(list, &count, NULL, NULL);
bool GenList_GetInfo(GENLIST, size_t *count, size_t *item_size, bool *do_memcpy);

// if (do_memcpy) item = &non_pointer_var
//           else item = pointer_var
bool GenList_Add(GENLIST, void *item);
bool GenList_AddAt(GENLIST, size_t i, void *item);

// if (do_memcpy) item = &non_pointer_var
//           else item = &pointer_var
bool GenList_GetAt(GENLIST, size_t i, void **item);

bool GenList_Clear(GENLIST);
bool GenList_RemoveAt(GENLIST, size_t i);

bool GenList_ForEach(GENLIST, GENLIST_FOREACH_HANDLER);

/* Error message ------------- */

int GenList_LastErr();
#define GENLIST_ERR_UNEXPECTED_NULL     1
#define GENLIST_ERR_OUT_OF_RANGE        2

/* ------------- Error message */

#endif // _GENLIST_H
