#include "Cell.h"

Cell::Cell(sf::Vector2i _gridPos) :
gridPos(_gridPos),
shape(sf::Vector2f(size, size)),
state(false)
{	
	shape.setFillColor(sf::Color::Black);
	shape.setPosition(sf::Vector2f(gridPos) * size);
}

void Cell::update(){
	if (flag == "kill"){
		kill();
	}
	else if (flag == "birth"){
		birth();
	}
}

void Cell::birth(){
	state = true;
	shape.setFillColor(sf::Color::White);
}

void Cell::kill(){
	state = false;
	shape.setFillColor(sf::Color::Black);
}