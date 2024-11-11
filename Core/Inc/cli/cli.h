#ifndef __CLI_H__
#define __CLI_H__

#include "main.h"

#define MAX_TOKENS 5
#define MAX_TOKEN_LENGTH 32

// Uncomment if u want use precalculated cmd HASHES
// #define USE_PRECALCULATED_HASH 

#define HASH_PRIME 5381

#define RX_BUFFER_SIZE 64


typedef char (Tokens)[MAX_TOKENS-1][MAX_TOKEN_LENGTH];

typedef void (*CommandHandler)(Tokens);

typedef struct {
    const char *command_name;
    CommandHandler handler;
    uint32_t hash;
} Command;

void start_rx(void);
void process_command(void);
void parse_command(void);
void rejected_cmd();
void tokenize(const char *input, char args[MAX_TOKENS - 1][MAX_TOKEN_LENGTH], uint32_t *cmd_hash);
void clear_buffer(void);
void clear_input(void);
void clear_last_input(void);
uint32_t calculate_hash(const char *str);

#ifndef USE_PRECALCULATED_HASH

void populate_cmd_hash(void);

#endif

#endif