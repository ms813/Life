#include "Game.h"

Game::Game() :
window(sf::VideoMode(200, 200), "Life"),
gridSize(40, 40)
{
	for (int i = 0; i < gridSize.x; i++){
		std::vector<Cell> col;
		for (int j = 0; j < gridSize.y; j++){					
			col.push_back(Cell(sf::Vector2i(i, j)));				
		}		
		cells.push_back(col);
	}

	//view.setCenter(sf::Vector2f(gridSize / 2));
	//window.setView(view);

	cells[1][7].birth();

	cells[3][7].birth();
	cells[3][6].birth();

	cells[5][5].birth();
	cells[5][4].birth();
	cells[5][3].birth();

	cells[7][4].birth();
	cells[7][3].birth();
	cells[7][2].birth();

	cells[8][3].birth();

}

void Game::run(){

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (window.isOpen())
	{

		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;

		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processEvents();
			update(TimePerFrame);

		}		

		render();
	}

}

void Game::processEvents(){
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
	}
}

void Game::render(){
	window.clear();

	for (int i = 0; i < cells.size(); i++){
		for (int j = 0; j < cells[0].size(); j++){
			window.draw(cells[i][j].getShape());
		}
	}

	window.display();
}

void Game::update(sf::Time _timePerFrame){

	turn++;
	std::cout << "Turn " << turn << std::endl;

	for (int i = 0; i < cells.size(); i++){		
		for (int j = 0; j < cells[0].size(); j++){

			Cell *cell = &cells[i][j];
			int liveNeighbours = countLiveNeighbours(cell);			

			//if fewer than 2 neighbors, die of underpopulation
			if (liveNeighbours < 2 && cell->getState()){
				cells[i][j].setFlag("kill");
				if (logging){
					std::cout << "Cell " << cell->getGridPos() << " had " << liveNeighbours << " neighbour(s) so dies of underpopulation" << std::endl;
				}
			}

			//if 2 or 3 neighbours, live

			//if greater than 3 live neighbours, die of overcrowding
			if (liveNeighbours > 3 && cell->getState()){
				cells[i][j].setFlag("kill");
				if (logging){
					std::cout << "Cell " << cell->getGridPos() << " dies of overcrowding" << std::endl;
				}
			}

			if (liveNeighbours == 3 && !cell->getState()){
				cells[i][j].setFlag("birth");
				if (logging){
					std::cout << "Cell " << cell->getGridPos() << " is born" << std::endl;
				}
			}
		}
	}

	for (int i = 0; i < cells.size(); i++){
		for (int j = 0; j < cells[0].size(); j++){
			cells[i][j].update();
		}
	}
}

int Game::countLiveNeighbours(Cell *cell){

	int live = 0;

	sf::Vector2i pos = cell->getGridPos();
	
	//top left
	if (cells[gridWrap(pos.x - 1, gridSize.x)][gridWrap(pos.y - 1, gridSize.x)].getState()){
		live++;
	}

	//top center
	if (cells[gridWrap(pos.x, gridSize.x)][gridWrap(pos.y - 1, gridSize.x)].getState()){
		live++;
	}

	//top right
	if (cells[gridWrap(pos.x + 1, gridSize.x)][gridWrap(pos.y - 1, gridSize.x)].getState()){
		live++;
	}

	//center left
	if (cells[gridWrap(pos.x - 1, gridSize.x)][gridWrap(pos.y, gridSize.x)].getState()){
		live++;
	}

	//center right
	if (cells[gridWrap(pos.x + 1, gridSize.x)][gridWrap(pos.y, gridSize.x)].getState()){
		live++;
	}

	//bottom left
	if (cells[gridWrap(pos.x - 1, gridSize.x)][gridWrap(pos.y + 1, gridSize.x)].getState()){
		live++;
	}

	//bottom center
	if (cells[gridWrap(pos.x, gridSize.x)][gridWrap(pos.y + 1, gridSize.x)].getState()){
		live++;		
	}

	//bottom right
	if (cells[gridWrap(pos.x + 1, gridSize.x)][gridWrap(pos.y + 1, gridSize.x)].getState()){
		live++;
	}	
	
	return live;
}

int Game::gridWrap(int coord, int gridDim){
	if (coord < 0){
		return gridDim - 1;
	} else if (coord > gridDim-1){
		return 0;		
	}
	else{
		return coord;
	}
}

std::ostream& operator<<(std::ostream& os, const sf::Vector2i v)
{
	os << '(' << v.x << ',' << v.y << ')';
	return os;
}