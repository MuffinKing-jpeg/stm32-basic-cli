#ifndef __CMD_LIST_H__
#define __CMD_LIST_H__

#include "cli.h"
#include "toggle_pin.h"

#define CMD_UNKNOWN_STR  "\0"

typedef enum {
    CMD_LED,
    CMD_CLEAR,
    QTY_CMD //Must be last element
} CommandID;

Command command_table[QTY_CMD]  = {
    [CMD_LED] = {
        "led",
        cmd_led,
        0
    },
    [CMD_CLEAR] = {
        "clear",
        cmd_clear,
        0
    }
};

#endif