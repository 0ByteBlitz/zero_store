#ifndef UTILS_H
#define UTILS_H

unsigned int hash(const char *key, unsigned int table_size);

// Splits a command string into up to 3 tokens: command, key, value. Returns number of tokens found.
int split_command(char *input, char **cmd, char **key, char **value);

#endif
