#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashMap.h"

map_t* init_map(){
    map_t* hashmap = malloc(sizeof(map_t));
    if (hashmap == NULL){
        return NULL;
    }

    hashmap->currlen = 0;
    hashmap->capacity = INITIAL_CAPACITY;
    hashmap->items = calloc(hashmap->capacity, sizeof(item));

    if (hashmap->items == NULL){
        free(hashmap);
        return NULL;
    }
    return hashmap;
}

int hashcode(map_t *map, const char *s){
    int hash = 0;
    int multiplier = M;
    for (int i = 0; s[i] != '\0'; i++){
        hash = (hash +(s[i]*multiplier)%map->capacity)%map->capacity;
        multiplier = (multiplier * M)%map->capacity;
    }
    return hash;
}

int map_hash(map_t *map, const char *key){
    int curr; int i; 

    curr = hashcode(map, key);

    //Linear Probe
    for(i = 0; i < CHAIN_LEN; i++){
        if(map->items[curr].isUsed == false){
            return curr;
        }
        if(map->items[curr].isUsed == true && (strcmp(map->items[curr].key,key)==0)) {
			return curr;
        }

        //go next
        curr = (curr + 1)% map->capacity;
    }  
    return MAP_FULL;
}

int map_rehash(map_t *map){
    int oldSize; item* curr;

    item* temp = (item *) calloc(2*map->capacity,sizeof(item));
    if (temp == NULL) return MAP_NOMEM;

    // change array
    curr = map->items;
    map->items = temp;

    //adjust size
    oldSize = map->capacity;
    map->capacity = 2 * map->capacity;
    map->currlen = 0;

    //rehashing
    for (int i = 0; i < oldSize; i++){
        if (curr[i].isUsed == false){
            continue;
        }
        int stat;
        stat = map_set(map, curr[i].key, curr[i].value);
        if (stat != MAP_OK){
            return stat;
        } 
    }

    free(curr);
    return MAP_OK;
}

int map_get(map_t *map, const char* key){
    int curr = hashcode(map, key);

    for (int i = 0; i < CHAIN_LEN; i++){
        bool isUsed = map->items[curr].isUsed;
        if(isUsed == true){
            if (strcmp(map->items[curr].key, key)==0){
                return map->items[curr].value;
            }
        }
        curr = (curr + 1)%map->capacity;
    }
    return 0;
}

int map_set(map_t *map, const char *key, int val) {

    int index = map_hash(map, key);

    while(index == MAP_FULL){
        if (map_rehash(map) == MAP_NOMEM){
            return MAP_NOMEM;
        }
        index = map_hash(map, key);
    }
    if (map->items[index].isUsed == false){
        map->currlen++;
    }

    map->items[index].key = strdup(key);
    map->items[index].value = val;
    map->items[index].isUsed = true;
    
    if (map->items[index].key == NULL) {
        return MAP_NOMEM;
    }
    return MAP_OK;
}



void map_destroy(map_t* map){
    int length = map_length(map);
    for (int i = 0; i < length; i++){
        if (map->items[i].isUsed){
            free(map->items[i].key);
        }
    }
    free(map->items);
    free(map);
    return;
}


int map_length(map_t* map){
    if (map!= NULL){
        return map->currlen;
    }
    return 0;
}


int map_inc(map_t *map, const char* key){
    int value = map_get(map, key);
    if(value <= 0){
        map_set(map, key, 1);
    } else{
        value++;
        map_set(map, key, value);
    }
    return value;
}

//This prints the map (UNSORTED: Only used for testing)
void printMap(map_t *map){
    int len = map->capacity;
    item* itemList = map->items;
    for(int i = 0; i < len; i++){
        if(itemList[i].value > 0){
            printf("%s %d\n", itemList[i].key, itemList[i].value);
        }
    }
}