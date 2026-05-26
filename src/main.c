#include "bob.h"
#include "raylib.h"
#include <config.h>
#include <packed.h>

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
  g_font = GetFontDefault();

  // Textures init
  Texture2D overlay_tex = {0};
  if (!load_texture_from_data(&g_asset_manager, "overlay.png", OVERLAY_PNG_DATA,
                              OVERLAY_PNG_DATA_SIZE, &overlay_tex)) {
    return 1;
  };
  Texture2D screen_bg_tex = {0};
  if (!load_texture_from_data(
          &g_asset_manager, "screen_background.png", SCREEN_BACKGROUND_PNG_DATA,
          SCREEN_BACKGROUND_PNG_DATA_SIZE, &screen_bg_tex)) {
    return 1;
  };

  // Sprites init
  Sprite overlay_spr = {0};
  if (!init_sprite(&overlay_spr, overlay_tex, 1, 1)) {
    return 1;
  };
  Sprite screen_bg_spr = {0};
  if (!init_sprite(&screen_bg_spr, screen_bg_tex, 1, 1)) {
    return 1;
  };

  Bob bob = make_bob(
      v2(w / 2.f, g_play_bounds.y + g_play_bounds.height - BOB_DEFAULT_RADIUS),
      KEY_LEFT, KEY_RIGHT, KEY_SPACE);

  /// DEBUG UI
  UI ui = UI_make(get_default_ui_theme(), &g_font, v2xx(0), "DEBUG", &g_mpos);

  while (!WindowShouldClose()) {
    g_delta = GetFrameTime();
    g_mpos = get_mpos_scaled();
    begin_frame();

    /// UI
    UI_begin(&ui, UI_LAYOUT_KIND_VERT);
    UI_text(&ui, "Hi", 16, WHITE);

    /// Input
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_F)) {
      ToggleFullscreen();
    }
    control_bob(&bob, g_delta);
    /// Update
    bound_bob_to_bounds(&bob, g_play_bounds);

    /// Draw
    ClearBackground(GetColor(0x181818FF));
    draw_sprite(&screen_bg_spr);
    draw_bob(&bob);

    if (g_debug) {
      DrawRectangleLinesEx(g_play_bounds, 1.f, WHITE);
      UI_draw(&ui);
    }

    draw_sprite(&overlay_spr);

    UI_end(&ui);

    end_frame();
  }

  // Sprites deinit
  free_sprite(&overlay_spr);
  free_sprite(&screen_bg_spr);

  clean_asset_manager(&g_asset_manager);
  UI_free(&ui);
  close_window();

  return 0;
}