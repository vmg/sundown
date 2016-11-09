/*
 * src_map.c
 * snowcrash
 *
 * Created by Zdenek Nemec on 4/21/13.
 * Copyright (c) 2013 Apiary Inc. All rights reserved.
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
    size_t i = 0;

    if (!map)
        return;
    
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
    size_t i;
    size_t first_item = -1;
    src_map *new_map = NULL;
    size_t remain_len = 0;
    
    if (!map ||
        !map->size ||
        !r ||
        !r->len)
        return NULL;

    /* find first item */
	for (i = 0; i < (size_t)map->size; ++i) {
        
        range *it = (range *)map->item[i];
        if (r->loc >= it->loc &&
            r->loc < it->loc + it->len) {
            first_item = i;
            break;
        }
    }
    
    if (first_item == (size_t)-1)
        return NULL;
    
    /* create new map */
    new_map = src_map_new();
    remain_len = r->len;
    
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
    range *new_range = NULL;
    
    if (!map ||
        !r ||
        !r->len)
        return;
    
    if (map->size) {
        /* check continuous range */
        range *last_range = (range *)map->item[map->size - 1];
        if (r->loc <= last_range->loc + last_range->len) {
            last_range->len += r->len - (last_range->loc + last_range->len - r->loc);
            return;
        }
    }

    /* not continuous, create new range and push */
    new_range = range_new(r->loc, r->len);
    stack_push(map, new_range);
}

/* return index-th cursor from map */
size_t
src_map_location(const src_map *map, size_t index)
{
    size_t i = 0;
    size_t cur = 0;
    
    if (!map ||
        !map->size)
        return -1;
    
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
src_map_new_tail(const src_map *map, size_t index, size_t maxlen)
{
    size_t i = 0;
    size_t cur = 0;
    size_t first_item = -1;
    size_t count = 0;
    src_map *new_map = NULL;
    
    if (!map ||
        !map->size)
        return NULL;
    
	for (i = 0; i < (size_t)map->size; ++i) {
        
        range *it = (range *)map->item[i];
        if (index < cur + it->len) {
            first_item = i;
            break;
        }
        cur += it->len;
    }

    if (first_item == (size_t)-1)
        return NULL;
    
    /* create new map */
    new_map = src_map_new();
	for (i = first_item; i < map->size && count < maxlen; ++i) {

        range *it = (range *)map->item[i];
        range *add_range = range_new(it->loc, it->len);
        
        if (count + it->len > maxlen) {
            add_range->len = maxlen - count;
        }
        count += it->len;
        stack_push(new_map, add_range);   
    }
    
    return new_map;
}
