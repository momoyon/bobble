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
  Texture2D joystick_tex = {0};
  if (!load_texture_from_data(&g_asset_manager, "joystick.png",
                              JOYSTICK_PNG_DATA, JOYSTICK_PNG_DATA_SIZE,
                              &joystick_tex)) {
    return 1;
  };
  Texture2D fire_button_tex = {0};
  if (!load_texture_from_data(&g_asset_manager, "fire_button.png",
                              FIRE_BUTTON_PNG_DATA, FIRE_BUTTON_PNG_DATA_SIZE,
                              &fire_button_tex)) {
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
  Sprite joystick_spr = {0};
  if (!init_sprite(&joystick_spr, joystick_tex, 2, 1)) {
    return 1;
  };
  joystick_spr.origin = v2(22, 78);
  joystick_spr.pos = v2_add(v2(106, 509), joystick_spr.origin);
  Sprite fire_button_spr = {0};
  if (!init_sprite(&fire_button_spr, fire_button_tex, 1, 1)) {
    return 1;
  };
  fire_button_spr.pos = v2(265, 579);

  Bob bob = make_bob(
      v2(w / 2.f, g_play_bounds.y + g_play_bounds.height - BOB_DEFAULT_RADIUS),
      KEY_LEFT, KEY_RIGHT, KEY_Z, &joystick_spr, &fire_button_spr);

  /// DEBUG UI
  UI ui = UI_make(get_default_ui_theme(), &g_font, v2xx(0), "DEBUG", &g_mpos);

  while (!WindowShouldClose()) {
    g_delta = GetFrameTime();
    g_mpos = get_mpos_scaled();
    begin_frame();

    /// UI
    UI_begin(&ui, UI_LAYOUT_KIND_VERT);
    UI_text(&ui, TextFormat("Bob.joystick_rotation: %f", bob.joystick_rotation), 16, WHITE);
    UI_text(&ui, TextFormat("Bob.joystick_rotation_target: %f", bob.joystick_rotation_target), 16, WHITE);

    /// Input
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_F)) {
      ToggleFullscreen();
    }
    control_bob(&bob, g_delta);
    /// Update
    bound_bob_to_bounds(&bob, g_play_bounds);
    update_bob(&bob, g_delta);

    /// Draw
    ClearBackground(GetColor(0x181818FF));
    draw_sprite(&screen_bg_spr);
    draw_bob(&bob);

    draw_sprite(&overlay_spr);
    draw_sprite(&joystick_spr);
    draw_sprite(&fire_button_spr);

    if (g_debug) {
      DrawRectangleLinesEx(g_play_bounds, 1.f, WHITE);
      UI_draw(&ui);
    }

    UI_end(&ui);

    end_frame();
  }

  // Sprites deinit
  free_sprite(&overlay_spr);
  free_sprite(&screen_bg_spr);
  free_sprite(&joystick_spr);
  free_sprite(&fire_button_spr);

  clean_asset_manager(&g_asset_manager);
  UI_free(&ui);
  close_window();

  return 0;
}