#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "blob.h"

void *blob_new(uint32_t size) {
    if (size < sizeof(uint32_t)) {
        return NULL;
    }
    // allocate a new blob_t of size
    blob_t *blob = (blob_t *)malloc(size + sizeof(blob_t));
    blob->cap = size;
    blob_reset(blob);
    return blob;
}

void blob_reset(blob_t *blob) {
    blob->pos = sizeof(blob_t);
    uint32_t *len = (uint32_t *)((uintptr_t)blob + blob->pos);
    *len = 0;
}

void *blob_append(blob_t *blob, uint32_t size) {
    // append to a blob
    size += sizeof(uint32_t);
    if (blob->pos + size + sizeof(uint32_t) > blob->cap) {
        return NULL;
    }
    uintptr_t pos = (uintptr_t)blob + blob->pos;
    uint32_t *len = (uint32_t *)pos;
    uintptr_t data = (pos + sizeof(uint32_t));
    pos += size;

    uint32_t *next = (uint32_t *)pos;
    *len = size;
    *next = 0;
    return (void *)data;
}

void *blob_extend(blob_t *blob, uint32_t size) {
    // append to a blob, resizing if necessary
    void *ret = blob_append(blob, size);
    if (ret == NULL) {
        blob->cap *= 2;
        blob_t *new = realloc(blob, blob->cap + sizeof(blob_t));
        if (new == NULL) {
            fprintf(stderr, "Warning: blob_extend() failed\n");
            return NULL;
        }
        ret = blob_append(blob, size);
    }
    return ret;
}

void *blob_next(blob_t *blob) {
    // return the next blob element (only use this if you've already started iteration)
    uintptr_t pos = (uintptr_t)blob + blob->pos;
    uint32_t len = *(uint32_t *)pos;
    if (len == 0) {
        return NULL;
    }
    void *next = (void *)(pos + sizeof(uint32_t));
    blob->pos += len;
    return next;
}

void blob_iter(blob_t *blob) {
    // start iteration on a blob_t (this is irreversible)
    blob->pos = sizeof(blob_t);
}
