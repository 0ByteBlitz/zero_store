#include "cache.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct KeyValue {
    char *key;
    char *value;
    struct KeyValue *next; // For maintaining collisions
} KeyValue;

#define TABLE_SIZE 128
static KeyValue *table[TABLE_SIZE];

// Simple hash function (using djb2)
static unsigned int hash(const char *key) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % TABLE_SIZE;
}

// Function to set key
void set_key(const char *key, const char *value) {
    unsigned int idx = hash(key);
    KeyValue *curr = table[idx];

    // Check if the key already exists, update if found
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            free(curr->value);
            curr->value = strdup(value);
            return;
        }
        curr = curr->next;
    }

    // If key is not found, create new entry
    KeyValue *new_entry = malloc(sizeof(KeyValue));
    new_entry->key = strdup(key);
    new_entry->value = strdup(value);
    new_entry->next = table[idx];
    table[idx] = new_entry;
}


// Function to get value by key
const char* get_key(const char *key) {
    unsigned int idx = hash(key);
    KeyValue *curr = table[idx];
    while (curr) {
        if(strcmp(curr->key, key) == 0) {
            return curr->value;
        }
        curr = curr->next;
    }
    return NULL;
}

// Function to delete by key 
void del_key(const char *key) {
    unsigned int idx = hash(key);
    KeyValue *curr = table[idx];
    KeyValue *prev = NULL;
    while (curr) {
        if(strcmp(curr->key, key) == 0) {
            if(prev) {
                prev->next = curr->next;
            } else {
                table[idx] = curr->next;
                free(curr->key);
                free(curr->value);
                free(curr);
            }
            prev = curr;
            curr = curr->next;
        }
    }
}


// Clean up the memory
void free_cache() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        KeyValue *curr = table[i];
        while(curr) {
            KeyValue *next = curr->next;
            free(curr->key);
            free(curr->value);
            free(curr);
            curr = next;
        }
        table[i] = NULL;
    }
}