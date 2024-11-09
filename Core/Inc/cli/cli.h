#ifndef __CLI_H__
#define __CLI_H__

#define RX_BUFFER_SIZE 64

void startRx(void);
void processCommand(void);
void parseCommand(void);

#endif