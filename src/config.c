#include "state.h"
#include <config.h>

int g_screen_width = 1280;
int g_screen_height = 720;

int g_window_width = 0;
int g_window_height = 0;

float g_screen_scale = 1.f;

Asset_manager g_asset_manager = {0};
float g_delta = 0.f;
Vector2 g_mpos = {0};

Rectangle g_play_bounds = {
  .x = 32*2.f,
  .y = 32*2.f,
  .width = 32*36,
  .height = 32*14,
};
State g_state = STATE_MAIN_MENU;
Font g_font = {0};


#ifdef DEBUG
bool g_debug = true;
#else
bool g_debug = false;
#endif
