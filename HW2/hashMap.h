#ifndef HASHMAP_H
#define HASHMAP_H

#define MAP_MISSING -3  // no element in map
#define MAP_NOMEM -2 	// no memory 
#define MAP_FULL -1 	// map is full
#define MAP_OK 0 	    // good

#define INITIAL_CAPACITY 512
#define M 10099
#define CHAIN_LEN 8

typedef struct {
    char* key;
    int value;
    bool isUsed;
} item;

typedef struct {
    item* items;
    int capacity;
    int currlen;
} map_t;


map_t* init_map();
int map_get(map_t *map, char* key);
int map_set(map_t *map, char *key, int val);
void map_destroy(map_t* map);
int map_length(map_t* map);
    
#endif