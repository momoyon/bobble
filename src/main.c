#include "bob.h"
#include "raylib.h"
#include <config.h>

#define ENGINE_IMPLEMENTATION
#include "engine.h"

#define COMMONLIB_IMPLEMENTATION
#define COMMONLIB_REMOVE_PREFIX
#include "commonlib.h"

int main(void) {
  int w, h;
  if (!init_window(g_screen_width, g_screen_height, g_screen_scale, "Bobble",
                   &g_window_width, &g_window_height)) {
    return 1;
  }
  SetTargetFPS(TARGET_FPS);
  w = g_window_width;
  h = g_window_height;

  g_play_bounds = CLITERAL(Rectangle){
      32.f,
      32.f,
      w - (32.f * 2),
      h - (32.f * 2),
  };

  Bob bob = make_bob(v2(w / 2.f, g_play_bounds.y + g_play_bounds.height - BOB_DEFAULT_RADIUS), KEY_LEFT, KEY_RIGHT, KEY_SPACE);

  while (!WindowShouldClose()) {
    g_delta = GetFrameTime();
    begin_frame();

    /// Input
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_F)) {
      ToggleFullscreen();
    }
    control_bob(&bob, g_delta);
    /// Update
    bound_bob_to_bounds(&bob, g_play_bounds);
    
    /// Draw
    ClearBackground(GetColor(0x181818FF));
    draw_bob(&bob);

    if (g_debug) {
      DrawRectangleLinesEx(g_play_bounds, 1.f, WHITE);
    }

    end_frame();
  }

  close_window();

  return 0;
}
