#ifndef BOB_H_
#define BOB_H_

#include "engine.h"

typedef struct Bob {
  Vector2 pos;
  float speed;
  float radius;

  // Controls
  int left_key;
  int right_key;
  int fire_key;

  Sprite *joystick_spr;
  float joystick_rotation;
  float joystick_rotation_target;

  Sprite *fire_button_spr;
  float fire_button_press_y;
  float fire_button_press_y_offset;
} Bob;

Bob make_bob(Vector2 pos, int left_key, int right_key, int fire_key, Sprite *joystick_spr, Sprite *fire_button_spr);
void control_bob(Bob *b, float dt);
void bound_bob_to_bounds(Bob *b, Rectangle bounds);
void update_bob(Bob *b, float dt);
void draw_bob(Bob *b);

#endif // !BOB_H_
