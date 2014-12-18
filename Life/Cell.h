#ifndef CELL_H
#define CELL_H

#include <SFML\Graphics.hpp>
#include "CellStatus.h"

class Cell{
public:

	Cell(float _size, sf::Vector2i _gridPos);

	void birth();
	void kill();

	void setNextFlag(CellStatus _flag){
		nextFlag = _flag;
	};

	void clearNextFlag(){
		nextFlag = CellStatus::NONE;
	}

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
	sf::Vector2i gridPos;
	bool state;
	CellStatus nextFlag;	
};

#endif