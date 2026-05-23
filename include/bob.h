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
} Bob;

Bob make_bob(Vector2 pos, int left_key, int right_key, int fire_key);
void control_bob(Bob *b, float dt);
void bound_bob_to_bounds(Bob *b, Rectangle bounds);
void update_bob(Bob *b, float dt);
void draw_bob(Bob *b);

#endif // !BOB_H_
