#ifndef CELL_H
#define CELL_H

#include <SFML\Graphics.hpp>
#include <string>

class Cell{
public:

	Cell(sf::Vector2i _gridPos);

	void birth();
	void kill();

	void setFlag(std::string _flag){
		flag = _flag;
	};

	sf::RectangleShape getShape(){
		return shape;
	}

	void setGridPos(sf::Vector2i pos){
		gridPos = pos;
	}

	sf::Vector2i getGridPos(){
		return gridPos;
	}

	bool getState(){
		return state;
	}

	void update();

private:
	sf::RectangleShape shape;
	float size = 5.0f;
	sf::Vector2i gridPos;
	bool state;
	std::string flag;
};

#endif