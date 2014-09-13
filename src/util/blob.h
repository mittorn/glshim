#ifndef BLOB_H
#define BLOB_H

#include <stdbool.h>

typedef struct {
    uint32_t cap, pos;
} blob_t;

/*
    blob_t layout in memory:
    | capacity | position | data length | data | data length | data | length = 0 | unused space |
*/

bool blob_empty(blob_t *blob);
void *blob_append(blob_t *blob, uint32_t size);
void *blob_extend(blob_t *blob, uint32_t size);
void *blob_new(uint32_t size);
void *blob_next(blob_t *blob);
void blob_iter(blob_t *blob);
void blob_reset(blob_t *blob);

#endif
