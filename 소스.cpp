#include <bangtal>
using namespace bangtal;

ObjectPtr stone[8][8];

enum class State {
	BLANK,
	POSSIBLE,
	BLACK,
	WHITE

};
State stone_state[8][8];

enum class Turn {
	BLACK,
	WHITE
};
Turn turn = Turn::BLACK;


void setState(int x, int y, State state)
{
	switch (state) {
		case State::BLANK: stone[y][x]->setImage("images/blank.png"); break; 
		case State::POSSIBLE: stone[y][x]->setImage(turn == Turn::BLACK ? "images/black possible.png" : "images/white possible.png"); break;
		case State::BLACK: stone[y][x]->setImage("images/black.png"); break;
		case State::WHITE: stone[y][x]->setImage("images/white.png"); break;
	}
	stone_state[y][x] = state;
}

bool checkPossible(int x, int y, int dx, int dy)
{
	State self = turn == Turn::BLACK ? State::BLACK : State::WHITE;
	State other = turn == Turn::BLACK ? State::WHITE : State::BLACK;

	bool possible = false;


	for (x += dx, y += dx; x >= 0 && x < 8 && y >= 0 && y < 8; x += dx, y += dy)
	{
		if (stone_state[y][x] == other)	
		{
			possible = true;
		}
		else if (stone_state[y][x] == self) 
		{
			return possible;
		}
		else {
			return false;
		}
	}

	return false;
}

bool checkPossible(int x, int y)
{
	if (stone_state[y][x] == State::BLACK) return false;
	if (stone_state[y][x] == State::WHITE) return false;
	setState(x, y, State::BLANK);

	int delta[8][2] = {
		{0,1},
		{1,1},
		{1,0},
		{1,-1},
		{0,-1},
		{-1,-1},
		{-1,0},
		{-1,1},
	};


	bool possible = false;
	for (auto d : delta) {
		if (checkPossible(x, y, d[0], d[1])) possible = true;
	}
	return possible;
}

void reverse(int x, int y, int dx, int dy)
{
	State self = turn == Turn::BLACK ? State::BLACK : State::WHITE;
	State other = turn == Turn::BLACK ? State::WHITE : State::BLACK;

	bool possible = false;


	for (x += dx, y += dx; x >= 0 && x < 8 && y >= 0 && y < 8; x += dx, y+= dx)
	{
		if (stone_state[y][x] == other)	
		{
			possible = true;
		}
		else if (stone_state[y][x] == self)
		{
			if (possible) {
				for (x -= dx, y -= dx; x >= 0 && x < 8 && y >= 0 && y < 8; x -= dx, y -= dy)
				{
					if (stone_state[y][x] == other) {
						setState(x, y, self);
					}
					else {
						return;
					}
				}
			}
		}
		else {
			return;
		}
	}

	
}

void reverse(int x, int y)
{
	int delta[8][2] = {
		{0,1},
		{1,1},
		{1,0},
		{1,-1},
		{0,-1},
		{-1,-1},
		{-1,0},
		{-1,1},
	};


	bool possible = false;
	for (auto d : delta) {
		if (checkPossible(x, y, d[0], d[1])) possible = true;
	}

}



bool setPossible() 
{
	bool possible = false;
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if (checkPossible(x, y)) {
				setState(x, y, State::POSSIBLE);
				possible = true;
			}
		}
	}
	return possible;
}

int main() {
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	auto scene = Scene::create("Othello", "images/background.png");

	
	for (int y = 0; y < 8; y++) {
		for(int x = 0; x < 8; x++) {
			stone[y][x] = Object::create("images/blank.png", scene, 40 + x * 80, 40 + y * 80);
			stone[y][x]->setOnMouseCallback([&, x, y](ObjectPtr object, int , int , MouseAction ) -> bool {
				if (stone_state[y][x] == State::POSSIBLE) {
					if (turn == Turn::BLACK) {
						setState(x, y, State::BLACK);
						reverse(x, y);
						turn = Turn::WHITE;
					}
					else {
						setState(x, y, State::WHITE);
						reverse(x, y);
						turn = Turn::BLACK;
					}
					if (!setPossible()) {
						turn = turn == Turn::BLACK ? Turn::WHITE : Turn::BLACK;

						if (!setPossible()) {
							showMessage("End Game");
						}
					}
				}
					return true;
				});
			stone_state[y][x] = State::BLANK;
		}
	}
	





	setState(3, 3, State::BLACK);
	setState(4, 4, State::BLACK);
	setState(3, 4, State::WHITE);
	setState(4, 3, State::WHITE);

	setPossible();

	startGame(scene);

}