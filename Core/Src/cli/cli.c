#include "cli.h"
#include "cmd_list.h"
#include "usart.h"
#include "toggle_pin.h"
#include <string.h>

__attribute__((section(".ccmram"))) char command_buffer[RX_BUFFER_SIZE] = {0};
__attribute__((section(".ccmram"))) Tokens tokens;
uint16_t buffer_index = 0;
uint8_t rx_data = '\0';

uint32_t calculate_hash(const char *str)
{
    uint32_t hash = HASH_PRIME; // Initialize with a large prime number
    int c;

    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

void populate_cmd_hash()
{
    for (uint8_t i = 0; i < QTY_CMD; i++)
    {
        command_table[i].hash = calculate_hash(command_table[i].command_name);
    }
}

void start_rx()
{
    HAL_UART_Receive_DMA(&huart2, &rx_data, 1);
}

void process_command()
{

    if (buffer_index < RX_BUFFER_SIZE)
    {
        command_buffer[buffer_index] = rx_data;

        switch (rx_data)
        {
        case '\n':
        case '\r':
            parse_command();
            break;

        case '\b':
        case 0x7F:
            clear_last_input();
            break;
        case 0x03:
            clear_input();
            break;
        default:
            if (buffer_index < RX_BUFFER_SIZE - 1)
            {
                HAL_UART_Transmit_DMA(&huart2, &rx_data, 1);
                buffer_index++;
            }
            start_rx();
            break;
        }
    }
    else
    {
        command_buffer[buffer_index] = '\0';
    }
}

void parse_command()
{
    uint32_t cmd = 0;
    char args[MAX_TOKENS - 1][MAX_TOKEN_LENGTH] = {0};
    tokenize(command_buffer, args, &cmd);
    clear_buffer();
}

void tokenize(const char *input, char args[MAX_TOKENS - 1][MAX_TOKEN_LENGTH], uint32_t *cmd_hash)
{
    int arg_count = 0;

    char input_copy[RX_BUFFER_SIZE];
    strncpy(input_copy, input, sizeof(input_copy));
    input_copy[sizeof(input_copy) - 1] = '\0';

    // Tokenize the command
    char *token = strtok(input_copy, " ");
    if (token != NULL)
    {
        char command[MAX_TOKEN_LENGTH];
        command[MAX_TOKEN_LENGTH - 1] = '\0';
        *cmd_hash = calculate_hash(command);
        // Tokenize arguments
        while ((token = strtok(NULL, " ")) != NULL && arg_count < MAX_TOKENS - 1)
        {
            strncpy(args[arg_count], token, MAX_TOKEN_LENGTH);
            args[arg_count][MAX_TOKEN_LENGTH - 1] = '\0';
            arg_count++;
        }
    }
}

void rejected_cmd()
{
    char response[] = "\r\nUnknow command\r\n";
    while (!(USART2->ISR & USART_ISR_TC))
    {
    }
    HAL_UART_Transmit_DMA(&huart2, (uint8_t *)response, strlen(response));
    clear_buffer();
}

void clear_input()
{
    if (buffer_index > 0)
    {
        const char clear_seq[] = "\033[1J\033[H"; // Some legacy ASCII shit.
        HAL_UART_Transmit_DMA(&huart2, (uint8_t *)clear_seq, sizeof(clear_seq));
    }
    clear_buffer();
}

void clear_last_input()
{
    if (buffer_index > 0)
    {
        command_buffer[buffer_index] = 0;
        buffer_index--; // Remove last character
        command_buffer[buffer_index] = 0;
        char backspace_seq[] = "\b \b";
        HAL_UART_Transmit_DMA(&huart2, (uint8_t *)backspace_seq, strlen(backspace_seq));
    }
    start_rx();
}

void clear_buffer()
{
    buffer_index = 0;
    memset(command_buffer, 0, RX_BUFFER_SIZE);
    start_rx();
}
