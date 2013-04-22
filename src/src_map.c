/*
 * src_map.c
 * snowcrash
 *
 * Created by Zdenek Nemec on 4/21/13.
 * Copyright (c) 2013 Apiary.io. All rights reserved.
 */

#include "src_map.h"

#include <assert.h>

range *
range_new(size_t loc, size_t len)
{
    range *r;
    if((r = malloc(sizeof *r)) != NULL) {
        r->loc = loc;
        r->len = len;
    }
    
    return r;
}

void
range_release(range *r)
{
    if (!r)
        return;
    
    free(r);
    r = NULL;
}

src_map *
src_map_new()
{
    src_map *map;
    if ((map = malloc(sizeof *map)) != NULL) {
        stack_init(map, 4);
    }
    
    return map;
}

void
src_map_release(src_map *map)
{
    if (!map)
        return;
    
    size_t i;
	for (i = 0; i < (size_t)map->asize; ++i) {
		range_release(map->item[i]);
    }
    
    stack_free(map);
    free(map);
    map = NULL;
}

src_map *
src_map_new_submap(const src_map *map, const range *r)
{
    if (!map ||
        !map->size ||
        !r ||
        !r->len)
        return NULL;

    /* find first item */
    size_t i;
    size_t first_item = -1;
	for (i = 0; i < (size_t)map->size; ++i) {
        
        range *it = (range *)map->item[i];
        if (r->loc >= it->loc &&
            r->loc < it->loc + it->len) {
            first_item = i;
            break;
        }
    }
    
    if (first_item == -1)
        return NULL;
    
    /* create new map */
    src_map *new_map = src_map_new();
    size_t remain_len = r->len;
    
	for (i = first_item; i < (size_t)map->size; ++i) {

        range *it = (range *)map->item[i];
        size_t add_loc = (i == first_item) ? r->loc : it->loc;
        size_t add_len = (it->len < remain_len) ? it->len : remain_len;
        
        range *add_range = range_new(add_loc, add_len);
        stack_push(new_map, add_range);
        
        remain_len -= add_len;
        
        if (remain_len <= 0)
            break;
    }

    return new_map;
}

/* append range to src_map */
void
src_map_append(src_map *map, const range *r)
{
    if (!map ||
        !r ||
        !r->len)
        return;
    
    if (map->size) {
        /* check continuous range */
        range *last_range = (range *)map->item[map->size - 1];
        if (r->loc == last_range->loc + last_range->len) {
            last_range->len += r->loc;
            return;
        }
    }

    /* not continuous, create new range and push */
    range *new_range = range_new(r->loc, r->len);
    stack_push(map, new_range);
}

/* return index-th cursor from map */
size_t
src_map_location(const src_map *map, size_t index)
{
    if (!map ||
        !map->size)
        return -1;
    
    size_t i;
    size_t cur = 0;
	for (i = 0; i < (size_t)map->size; ++i) {

        range *it = (range *)map->item[i];
        if (index < cur + it->len) {
            return it->loc + index - cur;
        }
        cur += it->len;
    }

 	assert(0);
    return -1;
}

/* create new src_map from index onward */
src_map *
src_map_new_tail(const src_map *map, size_t index)
{
    if (!map ||
        !map->size)
        return NULL;
    
    size_t i;
    size_t cur = 0;
    size_t first_item = -1;
	for (i = 0; i < (size_t)map->size; ++i) {
        
        range *it = (range *)map->item[i];
        if (index < cur + it->len) {
            first_item = i;
            break;
        }
        cur += it->len;
    }

    if (first_item == -1)
        return NULL;
    
    /* create new map */
    src_map *new_map = src_map_new();
    
	for (i = first_item; i < map->size; ++i) {
        
        range *it = (range *)map->item[i];
        range *add_range = range_new(it->loc, it->len);
        stack_push(new_map, add_range);
        
    }
    
    return new_map;
}
