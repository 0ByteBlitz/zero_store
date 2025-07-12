#include "utils.h"
#include <string.h>

unsigned int hash(const char *key, unsigned int table_size) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % table_size;
}

// Splits input into up to 3 tokens: command, key, value. Modifies input in-place.
int split_command(char *input, char **cmd, char **key, char **value) {
    *cmd = NULL;
    *key = NULL;
    *value = NULL;
    char *saveptr;
    char *token = strtok_r(input, " ", &saveptr);
    if (!token) return 0;
    *cmd = token;
    token = strtok_r(NULL, " ", &saveptr);
    if (!token) return 1;
    *key = token;
    token = strtok_r(NULL, "", &saveptr); // Get the rest as value (may contain spaces)
    if (token) *value = token;
    if (*value) return 3;
    if (*key) return 2;
    return 1;
}
