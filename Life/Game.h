#ifndef GAME_H
#define GAME_H

#include <SFML\Graphics.hpp>

#include <vector>
#include <iostream>
#include <cmath>

#include "Cell.h"

class Game{

public:
	
	Game();

	void run();
	friend std::ostream& operator<<(std::ostream& os, const sf::Vector2i v);	

private:
	sf::RenderWindow window;
	sf::View view;
	
	sf::Vector2i gridSize;
	std::vector<std::vector<Cell>> cells;
	bool running = false;

	bool logging = false;

	int turn = 0;

	float cellSize = 5.0f;

	void processEvents();
	void handleInput();
	void render();
	void update(sf::Time);
	int countLiveNeighbours(Cell *cell);
	int gridWrap(int coord, int gridDim);

	const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);	
};

#endif