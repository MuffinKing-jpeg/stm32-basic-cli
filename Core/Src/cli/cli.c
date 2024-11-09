#include "cli.h"
#include "usart.h"
#include <string.h>

__attribute__((section(".ccmram"))) char command_buffer[RX_BUFFER_SIZE] = {0};
uint16_t buffer_index = 0;
uint8_t rx_data = '\0';
uint8_t rx_busy = 0;

void startRx()
{
    HAL_UART_Receive_DMA(&huart2, &rx_data, 1);
}

// HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart2){
//     if (huart2->Instance = USART2)
//     {
//         rx_busy = 0;
//     }

// }

void processCommand()
{

    if (buffer_index < RX_BUFFER_SIZE)
    {

        command_buffer[buffer_index] = rx_data;

        switch (rx_data)
        {
        case '\n':
        case '\r':
            NULL; // Just to prevent some strange lint warning
            parseCommand();
            break;

        case '\b':
        case 0x7F:
            if (buffer_index > 0)
            {
                command_buffer[buffer_index] = 0;
                buffer_index--; // Remove last character
                char backspace_seq[] = "\b \b";
                HAL_UART_Transmit_DMA(&huart2, (uint8_t *)backspace_seq, strlen(backspace_seq));
            }
            break;
        default:

            if (buffer_index < RX_BUFFER_SIZE - 1)
            {
                HAL_UART_Transmit_DMA(&huart2, &rx_data, 1);
                buffer_index++;
            }
            break;
        }

        startRx();
    }
    else
    {
        command_buffer[buffer_index] = '\0';
    }
}

void parseCommand()
{
    if (strcmp(command_buffer, "led\r") == 0)
    {
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin); // Turn on LED
        char response[] = "\r\nLED is toggled\r\n";
        while (!(USART2->ISR & USART_ISR_TC))
        {
            buffer_index = 0;
        }

        HAL_UART_Transmit_DMA(&huart2, (uint8_t *)response, strlen(response));
    }
    else
    {
        char newline_seq[] = "\r\n";
        HAL_UART_Transmit_DMA(&huart2, (uint8_t *)newline_seq, strlen(newline_seq));
    }
    buffer_index = 0;
    memset(command_buffer, 0, RX_BUFFER_SIZE);
}