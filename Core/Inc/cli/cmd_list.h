#ifndef __CMD_LIST_H__
#define __CMD_LIST_H__

#include "cli.h"
#include "toggle_pin.h"

#define CMD_UNKNOWN_STR  "\0"
#define QTY_CMD sizeof(CommandID)+1

typedef enum {
    CMD_UNKNOWN,  // For unknown commands
    CMD_LED
} CommandID;

Command command_table[QTY_CMD]  = {
    [CMD_UNKNOWN] = {
        "\0",
        rejected_cmd,
        0
    },
    [CMD_LED] = {
        "led",
        cmd_led,
        0
    }
};

#endif