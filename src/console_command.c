#include <console_command.h>
#include <config.h>

void dispatch_console_command(size_t cmd_id, String_array args) {
    log_debug("Dispatched console command %zu with args: ", cmd_id);
    for (int i = 0; i < args.count; ++i) {
        log_debug(" - %s", args.items[i]);
    }
    switch (cmd_id) {
        case CCMD_EXIT: {
            g_force_quit = true;
        } break;
        case CCMD_COUNT: 
        default:
        ASSERT(false, "UNREACHABLE!");
    }
}