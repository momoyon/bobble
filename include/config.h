#ifndef CONFIG_H_
#define CONFIG_H_

#include <engine.h>

extern int g_screen_width;
extern int g_screen_height;

extern int g_window_width;
extern int g_window_height;

extern float g_screen_scale;

extern float g_delta;

extern Rectangle g_play_bounds;

#define TARGET_FPS 60
#define BOB_DEFAULT_SPEED 200.f

#endif // !CONFIG_H_
