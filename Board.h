//David Rowe
//Minesweeper Clone Project

#pragma once
#include "Tiles.h"
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
using std::ifstream;
using std::string;

class Board
{
private:
	//variables
	int columns;
	int rows;
	int mines;
	int xPix;
	int yPix;
	bool endGame = false;
	bool debugActive = false;
	int clearTiles;
	int numClicks = 0;
	int flagClick = 0;
	int mineCount;
	string digits;
	int digit1;
	int digit2;
	int digit3;
	const static int COL = 50;
	const static int ROW = 50;
	Tiles* boardGrid[ROW][COL];
	int scoreBoard[11] = {0, 21, 42, 63, 84, 105, 126, 147, 168, 189, 210};
	ifstream fileObj;

public:
	//constructor
	Board();
	//methods
	void display(sf::RenderWindow* window);
	void boardClick(int x, int y, bool iClick);
	void landMines();
	void calcAdjMines();
	void recursionClick(int xPos, int yPos);
	void mineDisplay(bool endReset);
	void resetGame(int num);
	void mineBoardDisplay();
	void debugBoard();
	void debugActiveTog();
	void flagMines();
};

