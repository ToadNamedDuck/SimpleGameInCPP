//Make a struct for button state
struct Button_State {
	bool is_down;
	bool changed;
};

//These are the buttons we care about for our game.
enum {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_ENTER,
	BUTTON_W,
	BUTTON_S,
	BUTTON_ESCAPE,

	BUTTON_COUNT,//Should always be the last item, so that if we add items to this enum, we don't have to worry about changing a hard coded number somewhere.
};

struct Input {
	Button_State buttons[BUTTON_COUNT];
};