#ifndef CONFIG_H_
#define CONFIG_H_

#include <engine.h>
#include <state.h>

extern int g_screen_width;
extern int g_screen_height;

extern int g_window_width;
extern int g_window_height;

extern float g_screen_scale;

extern Asset_manager g_asset_manager;
extern float g_delta;
extern Vector2 g_mpos;

extern Rectangle g_play_bounds;
extern State g_state;
extern Font g_font;

extern bool g_debug;

#define TARGET_FPS 60
#define BOB_DEFAULT_SPEED 500.f
#define BOB_DEFAULT_RADIUS 32.f

#endif // !CONFIG_H_
