//David Rowe
//Minesweeper Clone Project

#include "Tiles.h"
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include <iostream>
using namespace std;

//constructor
Tiles::Tiles()
{
	xPos = 0;
	yPos = 0;
	xPix = 0;
	yPix = 0;
	adjMines = 0;
}

//set each tile object position
void Tiles::setPos(int x, int y)
{
	xPos = x;
	xPix = x * 32;
	yPos = y;
	yPix = y * 32;
}


//draw based on toggle settings from click event ***EVERYTHING HAPPENS HERE**** called over & over
void Tiles::display(sf::RenderWindow* window)
{
	//display all mines
	if (gameOver)
	{
		sf::Sprite revealTile(TextureManager::GetTexture("tile_revealed"));
		revealTile.setPosition(xPix, yPix);
		window->draw(revealTile);
		sf::Sprite mineTile(TextureManager::GetTexture("mine"));
		mineTile.setPosition(xPix, yPix);
		window->draw(mineTile);
	}
	//continue with game
	else
	{
		//display tile with flag if (flagToggle)
		if (flagToggle)
		{
			sf::Sprite hiddenTile(TextureManager::GetTexture("tile_hidden"));
			sf::Sprite flagTile(TextureManager::GetTexture("flag"));
			hiddenTile.setPosition(xPix, yPix);
			flagTile.setPosition(xPix, yPix);
			window->draw(hiddenTile);
			window->draw(flagTile);
		}
		//display revealed tile with left click
		else if (tileClick)
		{
			if (minePresent)
			{
				sf::Sprite revealTile(TextureManager::GetTexture("tile_revealed"));
				revealTile.setPosition(xPix, yPix);
				window->draw(revealTile);
				sf::Sprite mineTile(TextureManager::GetTexture("mine"));
				mineTile.setPosition(xPix, yPix);
				window->draw(mineTile);
			}
			else
			{
				//reveal tile with number of adjacent mines
				if (adjMines >= 1)
				{
					string numPath = "number_" + to_string(adjMines);
					sf::Sprite revealTile(TextureManager::GetTexture("tile_revealed"));
					revealTile.setPosition(xPix, yPix);
					window->draw(revealTile);
 					sf::Sprite numTile(TextureManager::GetTexture(numPath));
					numTile.setPosition(xPix, yPix);
					window->draw(numTile);
				}
				else
				{
					//reveal empty tile
					sf::Sprite revealTile(TextureManager::GetTexture("tile_revealed"));
					revealTile.setPosition(xPix, yPix);
					window->draw(revealTile);
				}
			}

		}
		//display hidden tile
		else
		{
			//create sprite and load texture into unordered map
			sf::Sprite hiddenTile(TextureManager::GetTexture("tile_hidden"));
			//set position of Tile object sprite texture
			hiddenTile.setPosition(xPix, yPix);
			//cout << "x: " << xPix << " y: " << yPix << endl;
			//draw sprite images
			window->draw(hiddenTile);		//draw tile object texture
		}
	}
	//display all mines via debug button
	if (debug)
	{
		sf::Sprite mineTile(TextureManager::GetTexture("mine"));
		mineTile.setPosition(xPix, yPix);
		window->draw(mineTile);
	}
}

//toggle flag display bool variable
void Tiles::flagTog()
{
	flagToggle = !flagToggle;
}

//set tile left click bool variable to true
void Tiles::tileTog()
{
	tileClick = true;
}

//set mine or not
void Tiles::mineTog()
{
	minePresent = true;
}

//set debug toggle for displaying mines
void Tiles::debugTog()
{
	debug = !debug;
}