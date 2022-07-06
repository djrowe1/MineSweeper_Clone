//David Rowe
//COP 3503 - Project 3 - Minesweeper

#include "Board.h"
#include "TextureManager.h"
#include <SFML/Graphics.hpp>
#include "Random.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

Board::Board()
{
	//load board/mine data from file
	string configLine;
	string filename = "boards/config.CFG";
	fileObj.open(filename);
	//read setup data for game
	getline(fileObj, configLine);
	columns = stoi(configLine);
	getline(fileObj, configLine);
	rows = stoi(configLine);
	getline(fileObj, configLine);
	mines = stoi(configLine);
	//close file
	fileObj.close();
	//create 2d (Tiles) array grid board
	//create tile objects and store them into 2d array for board app
	//vector< vector<Tiles>>* boardGrid = new vector<vector<Tiles>>(columns, vector<Tiles>(rows));
	//cout << "rows: " << rows << " columns: " << columns << endl;
	//display first COL then fill out ROWS
	for (int x = 0; x < columns; x++)
		for (int y = 0; y < rows; y++)
		{
			//create new Tiles object for game play
			boardGrid[x][y] = new Tiles;	//[ROWS][COLS]
			//set positions of Tiles objects
			boardGrid[x][y]->setPos(x, y);
			//cout << "x: " << x << " y: " << y << endl;
		}
	//set number of non-mine tiles to determine victory later in the game
	clearTiles = (columns * rows) - mines;
	//set random mine field
	landMines();
	//calculate adjacent mines for each Tiles object
	calcAdjMines();
}

//called over and over
void Board::display(sf::RenderWindow* window)
{
	//local variable
	string faceName;
	//***draw tile section based on Tile object settings***
	for (int x = 0; x < columns; x++)
		for (int y = 0; y < rows; y++)
		{
			//called over and over
			boardGrid[x][y]->display(window);
			//cout << "x: " << x << "y: " << y << endl;
		}
	
	if (endGame || (numClicks == clearTiles))
	{
		//determine face based on type of game ending
		if (numClicks == clearTiles)
		{
			faceName = "face_win";
			//CALL FUNCTION FOR FLAGGING MINES
			flagMines();
		}
		else
			faceName = "face_lose";
		//calculate position of image based on board size
		xPix = ((columns * 32) / 2) - 32;
		yPix = rows * 32;
		//create sprite and load texture into unordered map
		sf::Sprite endFace(TextureManager::GetTexture(faceName));
		//set position of Tile object sprite texture
		endFace.setPosition(xPix, yPix);
		//draw sprite image
		window->draw(endFace);		//draw tile object texture
	}
	else
	{
		//***draw happy face / reset button***
		//calculate position of image based on board size
		xPix = ((columns * 32) / 2) - 32;
		yPix = rows * 32;
		//create sprite and load texture into unordered map
		sf::Sprite smiley(TextureManager::GetTexture("face_happy"));
		//set position of Tile object sprite texture
		smiley.setPosition(xPix, yPix);
		//draw sprite image
		window->draw(smiley);		//draw tile object texture
	}
		//***draw debug button***
		xPix = ((columns * 32) / 2) + 96;
		//create sprite and load texture into unordered map
		sf::Sprite debug(TextureManager::GetTexture("debug"));
		//set position of Tile object sprite texture
		debug.setPosition(xPix, yPix);
		//draw sprite image
		window->draw(debug);		//draw tile object texture
		//***draw test button1***
		xPix = ((columns * 32) / 2) + 160;
		sf::Sprite test1(TextureManager::GetTexture("test_1"));
		test1.setPosition(xPix, yPix);
		window->draw(test1);
		//***draw test button2***
		xPix = ((columns * 32) / 2) + 224;
		sf::Sprite test2(TextureManager::GetTexture("test_2"));
		test2.setPosition(xPix, yPix);
		window->draw(test2);
		//***draw test button3***
		xPix = ((columns * 32) / 2) + 288;
		sf::Sprite test3(TextureManager::GetTexture("test_3"));
		test3.setPosition(xPix, yPix);
		window->draw(test3);
		//***draw digits scoreboard
		mineBoardDisplay();
		//check negative sign
		if (mineCount < 0)
		{
			TextureManager::LoadTexture("digits", sf::IntRect(scoreBoard[10], 0, 21, 32));
			sf::Sprite digital(TextureManager::GetTexture("digits", sf::IntRect(scoreBoard[10], 0, 21, 32)));
			digital.setPosition(1, yPix);
			window->draw(digital);
		}
		else
		{
			TextureManager::LoadTexture("digits", sf::IntRect(21, 0, 14, 32));
			sf::Sprite digital(TextureManager::GetTexture("digits", sf::IntRect(21, 0, 14, 32)));
			digital.setPosition(1, yPix);
			window->draw(digital);
		}
		//first digit
		TextureManager::LoadTexture("digits", sf::IntRect(scoreBoard[digit1], 0, 21, 32));
		sf::Sprite digital1(TextureManager::GetTexture("digits", sf::IntRect(scoreBoard[digit1], 0, 21, 32)));
		digital1.setPosition(22, yPix);
		window->draw(digital1);
		//second digit
		TextureManager::LoadTexture("digits", sf::IntRect(scoreBoard[digit2], 0, 21, 32));
		sf::Sprite digital2(TextureManager::GetTexture("digits", sf::IntRect(scoreBoard[digit2], 0, 21, 32)));
		digital2.setPosition(43, yPix);
		window->draw(digital2);
		//third digit
		TextureManager::LoadTexture("digits", sf::IntRect(scoreBoard[digit3], 0, 21, 32));
		sf::Sprite digital3(TextureManager::GetTexture("digits", sf::IntRect(scoreBoard[digit3], 0, 21, 32)));
		digital3.setPosition(64, yPix);
		window->draw(digital3);
}

//click operations - defualt is right - iClick == false
//determines where (which Tile object) the click occured
//and calls appropriate toggle method
void Board::boardClick(int x, int y, bool iClick)
{
	int xPos = x / 32;
	int yPos = y / 32;
	//right click - add flag
	if (y < rows * 32 && !iClick && boardGrid[xPos][yPos]->tileClick == false && endGame == false)	//can only flag hidden tiles
	{
		boardGrid[xPos][yPos]->flagTog();
		if (boardGrid[xPos][yPos]->flagToggle)
			flagClick++;
		else
			flagClick--;
		//recalculate mine display
		mineBoardDisplay();
		
	}
	//left click - reveal tile
	else if (y < rows * 32 && iClick && numClicks != clearTiles && endGame == false)
	{
		if (boardGrid[xPos][yPos]->minePresent && boardGrid[xPos][yPos]->flagToggle == false)	//nothing happens if there's a flag
		{
			//terminate game with loss
			endGame = true;
			mineDisplay(true);
		}
		else if (boardGrid[xPos][yPos]->flagToggle == false)	//nothing happens if there's a flag
			recursionClick(xPos, yPos);
	}
	//left click - reset button
	else if (y > rows * 32 && y < (rows * 32) + 64 && x > (((columns * 32) / 2) - 32) && x < (((columns * 32) / 2) + 32) && iClick)
	{
		resetGame(0);
	}
	//left click - Debug button
	else if (y > rows * 32 && y < (rows * 32) + 64 && x > (((columns * 32) / 2) + 96) && x < (((columns * 32) / 2) + 160) && iClick && !endGame)
	{
		//toggle debug active
		debugActiveTog();
		//toggle debug feature for each Tile object
		debugBoard();
	}
	//left click - Test #1 - resetGame(1)
	else if (y > rows * 32 && y < (rows * 32) + 64 && x > (((columns * 32) / 2) + 160) && x < (((columns * 32) / 2) + 224) && iClick)
	{
		resetGame(1);
	}
	//left click - Test #2
	else if (y > rows * 32 && y < (rows * 32) + 64 && x > (((columns * 32) / 2) + 224) && x < (((columns * 32) / 2) + 288) && iClick)
	{
		resetGame(2);
	}
	//left click - Test #3
	else if (y > rows * 32 && y < (rows * 32) + 64 && x > (((columns * 32) / 2) + 288) && x < (((columns * 32) / 2) + 352) && iClick)
	{
		resetGame(3);
	}
}

//setup random land mines
void Board::landMines()
{
	int mineCount = 0;
	while (mineCount < mines)
	{
		//generate random mine coordinates
		int xMine = Random::Int(0, columns - 1);
		int yMine = Random::Int(0, rows - 1);
		//search for repeated
		if (!boardGrid[xMine][yMine]->minePresent)
		{
			boardGrid[xMine][yMine]->mineTog();
			mineCount++;
		}
	}
}

//calculate number of adjacent mines
void Board::calcAdjMines()
{
	//iterate through game board tiles and count number of adjacent tiles
	for (int x = 0; x < columns; x++)
		for (int y = 0; y < rows; y++)
		{
			//local variable
			int mineCount = 0;	//reset each iteration
			if (x > 0)	//check left side
				if (boardGrid[x - 1][y]->minePresent)
					mineCount++;
			if (x < columns-1)	//check right side
				if (boardGrid[x + 1][y]->minePresent)
					mineCount++;
			if (y > 0)	//check above
				if (boardGrid[x][y - 1]->minePresent)
						mineCount++;
			if (y < rows-1)	//check below
				if (boardGrid[x][y + 1]->minePresent)
					mineCount++;
			if (x > 0 && y > 0)		//check diag-left/above
				if (boardGrid[x - 1][y - 1]->minePresent)
					mineCount++;
			if (x < columns - 1 && y > 0)	//check diag-right/above
				if (boardGrid[x + 1][y - 1]->minePresent)
					mineCount++;
			if (x > 0 && y < rows-1)	//check diag-left/below
				if (boardGrid[x - 1][y + 1]->minePresent)
					mineCount++;
			if (x < columns-1 && y < rows-1)	//check diag-right/below
				if (boardGrid[x + 1][y + 1]->minePresent)
					mineCount++;
			//set mine count for Tile object
			boardGrid[x][y]->adjMines = mineCount;
		}
}

//reveal surrounding tiles with no mines or adjacent mines
void Board::recursionClick(int xPos, int yPos)
{
	//return if tile object has already been clicked
	if (boardGrid[xPos][yPos]->tileClick)
		return;
	//return if flag is prsent
	if (boardGrid[xPos][yPos]->flagToggle)
		return;
	//toggle tile click
	boardGrid[xPos][yPos]->tileTog();
	//count clear tile clicks
	numClicks++;
	//return if mine is present
	if (boardGrid[xPos][yPos]->minePresent)
		return;
	//recusive case
	if (boardGrid[xPos][yPos]->adjMines == 0)
	{
		if (xPos > 0)	//check left side
			recursionClick(xPos - 1, yPos);
		if (xPos < columns - 1)	//check right side
			recursionClick(xPos + 1, yPos);
		if (yPos > 0)	//check above
			recursionClick(xPos, yPos - 1);
		if (yPos < rows - 1)	//check below
			recursionClick(xPos, yPos + 1);
		if (xPos > 0 && yPos > 0)		//check diag-left/above
			recursionClick(xPos - 1, yPos - 1);
		if (xPos < columns - 1 && yPos > 0)	//check diag-right/above
			recursionClick(xPos + 1, yPos - 1);
		if (xPos > 0 && yPos < rows - 1)	//check diag-left/below
			recursionClick(xPos - 1, yPos + 1);
		if (xPos < columns - 1 && yPos < rows - 1)	//check diag-right/below
			recursionClick(xPos + 1, yPos + 1);
	}
}

//display mines with loss or hide mines based on overload parameter
void Board::mineDisplay(bool endReset)
{
	//toggle gameover bool for all mine tile objects
	for (int x = 0; x < columns; x++)
		for (int y = 0; y < rows; y++)
		{
			if (boardGrid[x][y]->minePresent)
			{
				boardGrid[x][y]->gameOver = endReset;
			}
		}
}

//reset game
void Board::resetGame(int num)
{
	//rest game button
	endGame = false;
	//reset number of board clicks
	numClicks = 0;
	//reset number of flag clicks
	flagClick = 0;
	//rest mine display Tile objects
	//mineDisplay(false);
	//reset Tile object toggle variables
	for (int x = 0; x < columns; x++)
		for (int y = 0; y < rows; y++)
		{
			boardGrid[x][y]->gameOver = false;
			boardGrid[x][y]->flagToggle = false;
			boardGrid[x][y]->tileClick = false;
			boardGrid[x][y]->minePresent = false;
			boardGrid[x][y]->debug = false;
		}
	//reset random mine field based on reset button
	if (num == 0)
	{
		// load board / mine data from file for proper reset
		string configLine;
		string filename = "boards/config.CFG";
		fileObj.open(filename);
		//read setup data for game
		getline(fileObj, configLine);
		columns = stoi(configLine);
		getline(fileObj, configLine);
		rows = stoi(configLine);
		getline(fileObj, configLine);
		mines = stoi(configLine);
		//close file
		fileObj.close();
		landMines();
	}	
	else
	{
		string path = "";
		if (num == 1)
			path = "boards/testboard1.BRD";
		else if (num == 2)
			path = "boards/testboard2.BRD";
		else
			path = "boards/testboard3.BRD";
		//load board/mine data from file
		int mineCount = 0;
		int mine = 0;
		string configLine;
		string filename = path;
		fileObj.open(filename);
		for (int i = 0; i < rows; i++)
		{
			getline(fileObj, configLine);
			for (int j = 0; j < columns; j++)
			{
				//determine if mine is placed
				mine = stoi(configLine.substr(j, 1));
				if (mine == 1)
				{
					boardGrid[j][i]->mineTog();		//add mine if line integer is 1
					mineCount++;
				}
			}
		}
		//record number of mines for game play
		mines = mineCount;
		//close file
		fileObj.close();
	}
	//set number of non-mine tiles to determine victory later in the game
	clearTiles = (columns * rows) - mines;
	//recalculate adjacent mines for each Tiles object
	calcAdjMines();
	if (debugActive)
	{
		debugBoard();
	}
}

//display mine count based on number of mines and flags
void Board::mineBoardDisplay()
{
	//calculate mine display
	mineCount = mines - flagClick;
	if (mineCount < 100 && mineCount > -100)
	{
		digit2 = abs(mineCount) / 10;
		digit3 = abs(mineCount) % 10;
		digit1 = 0;
	}
	else
	{
		digit3 = abs(mineCount) % 10;
		digit2 = (abs(mineCount) / 10) % 10;
		digit1 = ((abs(mineCount) / 10) / 10) % 10;
	}
}

//activate board debug
void Board::debugBoard()
{
	// toggle debug bool for all mine tile objects
		for (int x = 0; x < columns; x++)
			for (int y = 0; y < rows; y++)
			{
				if (boardGrid[x][y]->minePresent)
				{
					//toggle display variables for each Tile object
					boardGrid[x][y]->debugTog();
				}
			}
}

//toggle debug-active variable
void Board::debugActiveTog()
{
	debugActive = !debugActive;
}

//display flags on all Tile objects with mines
void Board::flagMines()
{
	//toggle flags for Tile objects with mines
	for (int x = 0; x < columns; x++)
		for (int y = 0; y < rows; y++)
		{
			if (boardGrid[x][y]->minePresent)
			{
				//toggle display variables for each Tile object
				boardGrid[x][y]->flagToggle = true;
				flagClick = mines;
			}
		}
	mineBoardDisplay();
}