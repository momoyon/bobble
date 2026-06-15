#include "state.h"
#include <config.h>

int g_screen_width = 0;
int g_screen_height = 0;

int g_window_width = 0;
int g_window_height = 0;

float g_screen_scale = 0;

bool g_force_quit = false;

Asset_manager g_asset_manager = {0};
float g_delta = 0.f;
Vector2 g_mpos = {0};

// TODO: Move this to config when it can support structs OR just have play_bounds_x, _y, et 
Rectangle g_play_bounds = {
  .x = 32*2.f,
  .y = 32*2.f,
  .width = 32*36,
  .height = 32*14,
};

float g_gravity = 0;
State g_state = STATE_MAIN_MENU;
Font g_font = {0};
int g_font_size = 0;
bool g_update_paused = false;


#ifdef DEBUG
bool g_debug = true;
#else
bool g_debug = false;
#endif

const char *g_console_commands[] = {
  "exit",
  "elephant",
  "exciting"
};
size_t g_console_commands_count = ARRAY_LEN(g_console_commands);
