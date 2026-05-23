#include <config.h>

int g_screen_width = 1280;
int g_screen_height = 720;

int g_window_width = 0;
int g_window_height = 0;

float g_screen_scale = 1.f;

Rectangle g_play_bounds = {
  .x = 32.f,
  .y = 32.f,
  .width = 1280-(32.f*2.f),
  .height = 720-(32.f*2.f),
};

float g_delta = 0.f;
