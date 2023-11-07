#include "hashMap.h"

#define bucketSize 5000000
#define P 999998727899999
#define M 127383

typedef unsigned long long ull;

typedef struct map{
    int buckets[bucketSize];
} Map;



ull hashcode(const char *s){
    ull hash = 0;
    ull multiplier = M;
    for (int i = 0; s[i] != '\0'; i++){
        hash = (hash +(s[i]*multiplier)%P)%P;
        multiplier = (multiplier * M)%P;
    }
    return hash;
}

int get(Map *map, const char *key){
}

void set(Map *map, const char *key, int val) {
}

void insert() {

}


