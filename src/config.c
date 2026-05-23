#include <config.h>

int g_screen_width = 1280;
int g_screen_height = 720;

int g_window_width = 0;
int g_window_height = 0;

float g_screen_scale = 1.f;

Rectangle g_play_bounds = {
  .x = BOB_DEFAULT_RADIUS,
  .y = BOB_DEFAULT_RADIUS,
  .width = 1280-(BOB_DEFAULT_RADIUS*2.f),
  .height = 720-(BOB_DEFAULT_RADIUS*2.f),
};

float g_delta = 0.f;

#ifdef DEBUG
bool g_debug = true;
#else
bool g_debug = false;
#endif
