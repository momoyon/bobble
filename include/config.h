#ifndef CONFIG_H_
#define CONFIG_H_

#include <engine.h>
#include <state.h>

extern Config g_config;

extern int g_screen_width;
extern int g_screen_height;

extern int g_window_width;
extern int g_window_height;

extern float g_screen_scale;

extern int g_fabrik_iterations_in_one_frame;

extern bool g_force_quit;

extern Asset_manager g_asset_manager;
extern float g_delta;
extern Vector2 g_mpos;

extern Rectangle g_play_bounds;
extern float g_gravity;
extern State g_state;
extern Font g_font;
extern int g_font_size;
extern bool g_update_paused;

extern bool g_debug;
extern const char *g_console_commands[];
extern size_t g_console_commands_count;


extern float g_bob_default_speed;
extern float g_bob_default_radius;
extern int g_target_fps;
#endif // !CONFIG_H_
