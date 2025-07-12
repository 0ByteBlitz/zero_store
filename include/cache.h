#ifndef CACHE_H
#define CACHE_H

void set_key(const char *key, const char *value);
const char* get_key(const char *key);
void del_key(const char *key);
void free_cache();

#endif