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

  Bob bob = make_bob(v2(w/2.f, h * 0.8f), KEY_LEFT, KEY_RIGHT, KEY_SPACE);

  while (!WindowShouldClose()) {
    g_delta = GetFrameTime();
    begin_frame();

    /// Input
    control_bob(&bob, g_delta);
    /// Update
    ///
    /// Draw
    ClearBackground(GetColor(0x181818FF));
    draw_bob(&bob);

    end_frame();
  }
  

  close_window();

  return 0;
}
