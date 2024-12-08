#include "gpio.h"
#include "toggle_pin.h"

#include <string.h>
#include <stdio.h>

void cmd_led(Tokens args)
{
    if (strcmp(args[0],"on") == 0)
    {
        led_toggle(GPIO_PIN_SET);
    }
    if (strcmp(args[0],"off") == 0)
    {
        led_toggle(GPIO_PIN_RESET);
    }
    
}

void led_toggle(GPIO_PinState state)
{
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, state);
    char response[24] = {0};


    switch (state)
    {
    case GPIO_PIN_SET:
        sprintf(response, "Praise the sun!\r\n");
        HAL_UART_Transmit_DMA(&huart2, (uint8_t *)response, strlen(response));
        break;
    case GPIO_PIN_RESET:
        sprintf(response, "In to the darkness!\r\n");
        HAL_UART_Transmit_DMA(&huart2, (uint8_t *)response, strlen(response));
        break;
    }
    clear_buffer();
}