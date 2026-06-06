#ifndef BALL_H_
#define BALL_H_

#include <engine.h>

typedef struct Ball {
    Vector2 pos, vel, acc;

    float radius;
    Color tint;

    Sprite spr;
} Ball;

Ball make_ball(Vector2 pos, Texture2D tex);
void update_ball(Ball *b, float dt);
void apply_gravity_to_ball(Ball *b);
void draw_ball(Ball *b);

#endif // BALL_H_