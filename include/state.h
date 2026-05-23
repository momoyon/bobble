#ifndef STATE_H_
#define STATE_H_

typedef enum {
  STATE_MAIN_MENU,
  STATE_PLAY,
  STATE_OPTIONS,
  STATE_COUNT,
} State;

const char *state_as_str(const State);

#endif // !STATE_H_

