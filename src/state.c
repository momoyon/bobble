#include <state.h>
#include <config.h>

const char *state_as_str(const State state) {
  switch (state) {
    case STATE_MAIN_MENU: return "Main Menu"; 
    case STATE_PLAY: return "Play"; 
    case STATE_OPTIONS: return "Options"; 
    case STATE_COUNT:
    default: ASSERT(false, "UNREACHABLE!");
  }
}
