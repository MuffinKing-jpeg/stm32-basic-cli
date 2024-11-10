#include "usart.h"
#include <string.h>
#include "cli.h"

void cmd_led(){
    char response[] = "\r\nLed toggled\r\n";
    HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
    while (!(USART2->ISR & USART_ISR_TC))
        {
        }
    HAL_UART_Transmit_DMA(&huart2, (uint8_t *)response, strlen(response));
    clear_buffer();
}