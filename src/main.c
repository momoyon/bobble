#include "bob.h"
#include "raylib.h"
#include <ball.h>
#include <config.h>
#include <packed.h>
#include <console_command.h>

#define ENGINE_IMPLEMENTATION
#include "engine.h"

#define COMMONLIB_IMPLEMENTATION
#define COMMONLIB_REMOVE_PREFIX
#include "commonlib.h"

int main(void) {
  Config config = {0};
  if (!read_config(&config, "config.momo")) {
    return 1;
  }
  ASSERT(get_int_from_config(&config, "screen_width", &g_screen_width), ".");
  ASSERT(get_int_from_config(&config, "screen_height", &g_screen_height), ".");
  ASSERT(get_float_from_config(&config, "screen_scale", &g_screen_scale), ".");
  ASSERT(get_float_from_config(&config, "gravity", &g_gravity), ".");
  ASSERT(get_int_from_config(&config, "font_size", &g_font_size), ".");

  int w, h;
  if (!init_window(g_screen_width, g_screen_height, g_screen_scale, "Bobble",
                   &g_window_width, &g_window_height)) {
    return 1;
  }
  SetTargetFPS(TARGET_FPS);
  w = g_window_width;
  h = g_window_height;

  g_font = GetFontDefault();

  Console debug_console = make_console(
      CONSOLE_FLAG_READLINE_USES_UNPREFIXED_LINES, g_font, g_font_size * 2);
  float debug_console_y_off = debug_console.font_size;
  float debug_console_y = 0.f;
  float debug_console_height = g_window_height * 0.5f;
  float debug_console_target_y = -debug_console_height - debug_console_y_off;
  bool debug_console_active = false;

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

  /// @TEMP
  Vector2 play_bounds_size = v2(g_play_bounds.width, g_play_bounds.height);
  Ball b = make_ball(v2_add(v2(g_play_bounds.x, g_play_bounds.y),
                            v2_scale(play_bounds_size, 0.5)),
                     fire_button_tex);

  /// DEBUG UI
  UI ui = UI_make(get_default_ui_theme(), &g_font, v2xx(10), "DEBUG", &g_mpos);

  while (!WindowShouldClose() && !g_force_quit) {
    g_delta = GetFrameTime();
    g_mpos = get_mpos_scaled();
    begin_frame();

    /// UI
    UI_begin(&ui, UI_LAYOUT_KIND_VERT);
    UI_text(&ui, TextFormat("Bob.joystick_rotation: %f", bob.joystick_rotation),
            g_font_size, WHITE);
    UI_text(&ui,
            TextFormat("Bob.joystick_rotation_target: %f",
                       bob.joystick_rotation_target),
            g_font_size, WHITE);

    /// Input
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_F)) {
      ToggleFullscreen();
    }
    control_bob(&bob, g_delta);
    if (IsKeyPressed(KEY_F2)) {
      debug_console_active = !debug_console_active;
      if (debug_console_active) {
        debug_console_target_y = 0.f;
      } else {
        debug_console_target_y = -debug_console_height - debug_console_y_off;
      }
      g_update_paused = debug_console_active;
    }
    if (debug_console_active) {
      if (input_to_console(&debug_console, "", 0)) {
        char *input = get_current_console_line_buff(&debug_console);
        add_line_to_console_simple(&debug_console, input, WHITE, true);
        Ids matched_cmd_ids = match_command(input, g_console_commands, g_console_commands_count);

        if (matched_cmd_ids.count > 1) {
          for (int i = 0; i < matched_cmd_ids.count; ++i) {
            int matched_cmd_id = matched_cmd_ids.items[i];
            const char *potential_matched_cmd = g_console_commands[matched_cmd_id];
            log_info_console_color(debug_console, GRAY, " - %s", potential_matched_cmd);
          }
        } else if (matched_cmd_ids.count == 1) {
          log_info_console_color(debug_console, YELLOW, "Valid Command '%s'", input);
          String_array args = get_current_console_args(&debug_console);
          dispatch_console_command(matched_cmd_ids.items[0], args);
          for (int i = 0; i < args.count; ++i) {
            free((void *)args.items[i]);
          }
          darr_free(args);
        } else {
          log_error_console(debug_console, "Invalid Command '%s'", input);
        }

        darr_free(matched_cmd_ids);
        clear_current_console_line(&debug_console);
      }
    }

    /// Update
    bound_bob_to_bounds(&bob, g_play_bounds);
    if (!g_update_paused) {
      update_bob(&bob, g_delta);
      update_ball(&b, g_delta);
    }
    debug_console_y +=
        (debug_console_target_y - debug_console_y) * g_delta * 10.f;

    /// Draw
    ClearBackground(GetColor(0x181818FF));
    draw_sprite(&screen_bg_spr);
    draw_bob(&bob);
    draw_ball(&b);

    draw_sprite(&overlay_spr);
    draw_sprite(&joystick_spr);
    draw_sprite(&fire_button_spr);

    if (g_debug) {
      DrawRectangleLinesEx(g_play_bounds, 1.f, WHITE);
      UI_draw(&ui);

      Rectangle r = {
          .x = 0,
          .y = debug_console_y,
          .width = g_window_width,
          .height = g_window_height * 0.5,
      };
      if (debug_console_active) {
        DrawRectangle(0, 0, g_window_width, g_window_height, ColorAlpha(BLACK, 0.5));
      }
      draw_console(&debug_console, r, v2xx(2), GetColor(0x141414), WHITE, 1.0);
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
