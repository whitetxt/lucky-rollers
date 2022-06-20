typedef enum transition_type {
	TRANSITION_TYPE_CIRCLE,
	TRANSITION_TYPE_SWIPE_LEFT,
	TRANSITION_TYPE_SWIPE_RIGHT,
} transition_type;

extern int transition_timer;
extern int transition_duration;
extern bool transition_forwards;
extern game_state after_transition;
extern lerp_function transition_lerp;
extern transition_type trans_type;

void transition();