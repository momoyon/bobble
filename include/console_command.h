#ifndef MISC_H_
#define MISC_H_

#include <engine.h>

typedef enum Console_command_id {
    CCMD_EXIT,
    CCMD_SET_VAR,
    CCMD_COUNT
} Console_command_id;

void dispatch_console_command(size_t cmd_id, Console *console);

#endif // MISC_H_