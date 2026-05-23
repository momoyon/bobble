#include "raylib.h"
#include <bob.h>
#include <config.h>

Bob make_bob(Vector2 pos, int left_key, int right_key, int fire_key) {
  Bob res = {0};
  res.pos = pos;
  res.left_key = left_key;
  res.right_key = right_key;
  res.fire_key = fire_key;
  res.speed = BOB_DEFAULT_SPEED;
  res.radius = BOB_DEFAULT_RADIUS;

  return res;
}

void control_bob(Bob *b, float dt) {
  if (IsKeyDown(b->left_key)) {
    b->pos.x -= b->speed * dt;
  }

  if (IsKeyDown(b->right_key)) {
    b->pos.x += b->speed * dt;
  }
}

void bound_bob_to_bounds(Bob *b, Rectangle bounds) {
  if (b->pos.x < bounds.x + b->radius) {
    b->pos.x = bounds.x + b->radius;
  }

  if (b->pos.x > bounds.x + bounds.width - b->radius) {
    b->pos.x = bounds.x + bounds.width - b->radius;
  }

  if (b->pos.y < bounds.y + b->radius) {
    b->pos.y = bounds.y + b->radius;
  }

  if (b->pos.y > bounds.y + bounds.height - b->radius) {
    b->pos.y = bounds.y + bounds.height - b->radius;
  }
}

void update_bob(Bob *b, float dt) {
  (void)b;
  (void)dt;
}

void draw_bob(Bob *b) {
  DrawCircleV(b->pos, b->radius, RED);
}

