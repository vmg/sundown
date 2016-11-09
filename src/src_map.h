/*
 * src_map.h
 * snowcrash
 *
 * Created by Zdenek Nemec on 4/21/13.
 * Copyright (c) 2013 Apiary Inc. All rights reserved.
 *
 */

#ifndef SNOWCRASH_SUNDOWN_SRCMAP_H
#define SNOWCRASH_SUNDOWN_SRCMAP_H

#include <stdlib.h>
#include "stack.h"

#ifdef __cplusplus
extern "C" {
#endif
    
/* range: character data range */
typedef struct {
    size_t loc;
    size_t len;
} range;

/* range_new: allocate & init new range */
range *range_new(size_t loc, size_t len);

/* range_new: release range */
void range_release(range *r);

/* stack of range forming the buffer content */
typedef struct stack src_map;

/* src_map_new: allocate new source map */
src_map *src_map_new();
    
/* src_map_release: release source map */
void src_map_release(src_map *map);
    
/* src_map_submap: create map from a subset of map */
src_map *src_map_new_submap(const src_map *map, const range *r);

/* src_map_new_tail: create map from tail of map */
src_map *src_map_new_tail(const src_map *map, size_t index, size_t maxlen);
    
/* src_map_append: append range to source map */
void src_map_append(src_map *map, const range *r);
    
/* src_map_location: returns location in source for given index */
size_t src_map_location(const src_map *map, size_t index);

    
#ifdef __cplusplus
}
#endif

#endif
