#include "bob.h"
#include <config.h>
#include <console_command.h>
#include <engine.h>


// TODO: The way we match the commands is kinda wacky rn.
// If i change the g_console_commands[] in config.c, i have to mind to match
// that change to here...
void dispatch_console_command(size_t cmd_id, Console *console) {
  ASSERT(cmd_id < g_console_commands_count, "Cmd_id shouldn't be out of bounds!");
  String_array args = get_current_console_args(console);
  log_debug("Dispatched console command %zu with args: ", cmd_id);
  for (int i = 0; i < args.count; ++i) {
    log_debug(" - %s", args.items[i]);
  }

  ASSERT(CCMD_COUNT == g_console_commands_count,
         "Mismatch of g_console_commands and Console_command_id enum!");
  switch (cmd_id) {
  case CCMD_EXIT: {
    g_force_quit = true;
  } break;
  case CCMD_SET_VAR: {
    // TODO: Currently cannot set strings and chars because of how the argument parsing is done in the console.
    static bool warned_user_that_we_cant_set_strings_and_chars = false;
    if (!warned_user_that_we_cant_set_strings_and_chars) {
        log_warning_console(*console, "%s", "Currently cannot set strings and chars because of how the argument parsing is done in the console.");
        log_warning("%s", "Currently cannot set strings and chars because of how the argument parsing is done in the console.");
        warned_user_that_we_cant_set_strings_and_chars = true;
    }
    args.count--; // Remove cmd
    if (args.count < 2) {
        log_error_console(*console, "Command '%s' expects 2 arguments!", g_console_commands[cmd_id]);
        break;
    }
    // NOTE: args.items[0] is the command itself
    const char *key   = args.items[1];
    const char *value = args.items[2];
    String_view value_sv = SV(value);

    Config_value config_value = config_value_from_sv(value_sv, &g_config);

    // log_debug("Setting '%s' to '%s' (%s)", key, value, config_value_kind_as_str(config_value.kind));
    // log_debug_console(*console, "Setting '%s' to '%s' (%s)", key, value, config_value_kind_as_str(config_value.kind));
    if (set_value_to_config(&g_config, key, config_value)) { 
        log_debug_console(*console, "Set '%s' to '%s' (%s)", key, value, config_value_kind_as_str(config_value.kind));
        log_debug("Set '%s' to '%s' (%s)", key, value, config_value_kind_as_str(config_value.kind));
    }
  } break;
  case CCMD_COUNT:
  default:
    ASSERT(false, "UNREACHABLE!");
  }
  for (int i = 0; i < args.count; ++i) {
    free((void *)args.items[i]);
  }
  darr_free(args);
}