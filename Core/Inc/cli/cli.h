#ifndef __CLI_H__
#define __CLI_H__

#include "main.h"

#define MAX_TOKENS 5
#define MAX_TOKEN_LENGTH 20

#define HASH_PRIME 5381

#define RX_BUFFER_SIZE 64

typedef void (*CommandHandler)();

typedef char (Tokens)[MAX_TOKENS][MAX_TOKEN_LENGTH];

typedef struct {
    const char *command_name;
    CommandHandler handler;
    uint32_t hash;
} Command;

void start_rx(void);
void process_command(void);
void parse_command(void);
void rejected_cmd(void);
void populate_cmd_hash(void);
void tokenize(const char *input, char args[MAX_TOKENS - 1][MAX_TOKEN_LENGTH], uint32_t *cmd_hash);
void clear_buffer(void);
void clear_input(void);
void clear_last_input(void);
uint32_t calculate_hash(const char *str);

#endif