#ifndef MISC_H_
#define MISC_H_

#define COMMONLIB_REMOVE_PREFIX
#include <commonlib.h>

typedef enum Console_command_id {
    CCMD_EXIT,
    CCMD_COUNT
} Console_command_id;

void dispatch_console_command(size_t cmd_id, String_array args);

#endif // MISC_H_