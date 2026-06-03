#include "bob.h"
#include "config.h"
#include <ball.h>

Ball make_ball(Vector2 pos, Texture2D tex) {
  Ball res = {0};
  res.pos = pos;
  res.tint = WHITE;
  ASSERT(init_sprite(&res.spr, tex, 1, 1), "Ball.make_ball.init_sprite() fail");
  center_sprite_origin(&res.spr);
  res.radius = res.spr.height * 0.5;

  const float speed = 200.f;
  float dir = randomf(0.f, 1.f) >= 0.5f ? -1 : 1;
  res.vel.x = speed * dir;

  return res;
}

void update_ball(Ball *b, float dt) {
  b->spr.pos = b->pos;

  // Apply gravity
  apply_gravity_to_ball(b);

  // Physics
  b->vel = v2_add(b->vel, b->acc);
  b->pos = v2_add(b->pos, v2_scale(b->vel, dt));
  b->acc = v2xx(0);

  // Collision/Bounce with play bounds
  float bot = g_play_bounds.y + g_play_bounds.height;
  float top = g_play_bounds.y;
  float bounce_mod = 1.f;

  if (b->pos.y + b->radius > bot) {
    b->pos.y = bot - b->radius;
    b->vel.y *= -bounce_mod;
  } else if (b->pos.y - b->radius < top) {
    // b->pos.y = top + b->radius;
  }

  float right = g_play_bounds.x + g_play_bounds.width;
  float left = g_play_bounds.x;
  if (b->pos.x + b->radius > right) {
    b->pos.x = right - b->radius;
    b->vel.x *= -bounce_mod;
  } else if (b->pos.x - b->radius < left) {
    b->pos.x = left + b->radius;
    b->vel.x *= -bounce_mod;
  }
}

void apply_gravity_to_ball(Ball *b) { b->acc.y += g_gravity; }

void draw_ball(Ball *b) {
  draw_sprite(&b->spr);
  if (g_debug) {
    DrawCircleV(b->pos, b->radius, b->tint);
  }
}