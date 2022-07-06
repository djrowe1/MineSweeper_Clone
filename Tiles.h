//David Rowe
//Minesweeper Clone Project
#include <SFML/Graphics.hpp>
#pragma once
class Tiles
{
private:
	//variables
	int xPos;
	int yPos;
	int xPix;
	int yPix;

public:
	bool minePresent = false;
	bool tileClick = false;
	bool flagToggle = false;
	bool gameOver = false;
	bool debug = false;
	int adjMines;
	Tiles();
	void setPos(int x, int y);
	void display(sf::RenderWindow* window);				//draw tile texture
	void flagTog();
	void tileTog();
	void mineTog();
	void debugTog();
};

