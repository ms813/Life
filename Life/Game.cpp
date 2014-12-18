#include "Game.h"

Game::Game() :
window(sf::VideoMode(400, 400), "Life"),
cellSize(5.0f),
turnTime(sf::seconds(0.1f))
{
	gridSize = sf::Vector2i(window.getSize().x / cellSize, window.getSize().y / cellSize);
	for (int i = 0; i < gridSize.x; i++){
		std::vector<Cell> col;
		for (int j = 0; j < gridSize.y; j++){					
			col.push_back(Cell(cellSize, sf::Vector2i(i, j)));
		}		
		cells.push_back(col);
	}
}

void Game::run(){

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time turnClock = sf::Time::Zero;

	while (window.isOpen())
	{
		sf::Time elapsedTime = clock.restart();		
		timeSinceLastUpdate += elapsedTime;

		while (timeSinceLastUpdate > TimePerFrame)
		{
			turnClock += timeSinceLastUpdate;
			timeSinceLastUpdate -= TimePerFrame;			

			if (running && (turnClock > turnTime)){
				update(TimePerFrame);
				turnClock = sf::Time::Zero;
			}

			processEvents();
			handleInput();
		}		

		render();
	}

}

void Game::processEvents(){
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed){
			window.close();
		}	

		if (event.type == sf::Event::KeyPressed){
			handleKeyPress(event);
		}		
	}
}

void Game::handleKeyPress(sf::Event event){
	if (event.key.code == sf::Keyboard::Return){
		running = !running;		
	}

	if (event.key.code == sf::Keyboard::Escape){
		reset();
	}

	if (event.key.code == sf::Keyboard::Right){
		update(sf::Time());
	}
}

void Game::handleInput(){
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
		sf::Vector2i localPosition = sf::Mouse::getPosition(window);

		sf::Vector2i tileId(round(localPosition.x / cellSize), round(localPosition.y / cellSize));		

		if (tileId.x <= gridSize.x && tileId.x >= 0 && tileId.y <= gridSize.y && tileId.y >= 0){			
		
			if (!cells[tileId.x][tileId.y].getState()){
				//left clicked on a dead cell				
				cells[tileId.x][tileId.y].birth();
				if (logging){
					std::cout << "Cell at " << tileId << " birthed" << std::endl;
				}
			}			
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)){
		sf::Vector2i localPosition = sf::Mouse::getPosition(window);

		sf::Vector2i tileId(round(localPosition.x / cellSize), round(localPosition.y / cellSize));

		if (tileId.x <= gridSize.x && tileId.x >= 0 && tileId.y <= gridSize.y && tileId.y >= 0){

			if (cells[tileId.x][tileId.y].getState()){
				//right clicked on a live cell				
				cells[tileId.x][tileId.y].kill();
				if (logging){
					std::cout << "Cell at " << tileId << " killed" << std::endl;
				}
			}
		}
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
	if (logging){
		std::cout << "Turn " << turn << std::endl;
	}		

	for (int i = 0; i < cells.size(); i++){		
		for (int j = 0; j < cells[0].size(); j++){		
			Cell *cell = &cells[i][j];
			int liveNeighbours = countLiveNeighbours(cell);			

			//if fewer than 2 neighbors, die of underpopulation
			if (liveNeighbours < 2 && cell->getState()){
				cells[i][j].setNextFlag(CellStatus::KILL);
				if (logging){
					std::cout << "Cell " << cell->getGridPos() << " had " << liveNeighbours << " neighbour(s) so dies of underpopulation" << std::endl;
				}
			}

			//if 2 or 3 neighbours, live

			//if greater than 3 live neighbours, die of overcrowding
			if (liveNeighbours > 3 && cell->getState()){
				cells[i][j].setNextFlag(CellStatus::KILL);
				if (logging){
					std::cout << "Cell " << cell->getGridPos() << " dies of overcrowding" << std::endl;
				}
			}

			if (liveNeighbours == 3 && !cell->getState()){
				cells[i][j].setNextFlag(CellStatus::BIRTH);
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

void Game::reset(){
	running = false;
	turn = 0;

	for (int i = 0; i < gridSize.x; i++){
		for (int j = 0; j < gridSize.y; j++){
			cells[i][j].kill();			
		}
	}
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