#ifndef GAME_H
#define GAME_H

#include <SFML\Graphics.hpp>

#include <vector>
#include <iostream>

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

	bool logging = false;

	int turn = 0;


	void processEvents();
	void render();
	void update(sf::Time);
	int countLiveNeighbours(Cell *cell);
	int gridWrap(int coord, int gridDim);

	const sf::Time TimePerFrame = sf::seconds(1.f / 10.f);	
};

#endif