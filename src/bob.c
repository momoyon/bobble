#include "raylib.h"
#include <bob.h>
#include <config.h>

Bob make_bob(Vector2 pos, int left_key, int right_key, int fire_key, Sprite *joystick_spr) {
  Bob res = {0};
  res.pos = pos;
  res.left_key = left_key;
  res.right_key = right_key;
  res.fire_key = fire_key;
  res.speed = BOB_DEFAULT_SPEED;
  res.radius = BOB_DEFAULT_RADIUS;
  res.joystick_spr = joystick_spr;
  res.joystick_rotation = 0.f;
  res.joystick_rotation_target = 0.f;

  return res;
}

void control_bob(Bob *b, float dt) {
  float vel = 0.f;
  b->joystick_rotation_target = 0.f;
  const float D = 30.f;
  if (IsKeyDown(b->left_key)) {
    vel = -b->speed * dt;
    b->joystick_rotation_target = -D;
  }

  if (IsKeyDown(b->right_key)) {
    vel = b->speed * dt;
    b->joystick_rotation_target = D;
  }

  b->pos.x += vel;
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
  b->joystick_rotation += (b->joystick_rotation_target - b->joystick_rotation) * dt * 10.f;

  if (b->joystick_spr) {
    b->joystick_spr->rotation = b->joystick_rotation;
  }
}

void draw_bob(Bob *b) {
  DrawCircleV(b->pos, b->radius, RED);
}