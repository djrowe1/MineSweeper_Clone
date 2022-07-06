//David Rowe
//Minesweeper Clone
//Clone / rebuild the Minesweeper game utilizing SFML graphics

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Random.h"
#include "TextureManager.h"
#include <unordered_map>
#include "Board.h"

using namespace std;
int main()
{
    //test static Random class
    //int value = Random::Int(0, 50);
    //cout << value << endl;

    //test texture manager for SFML Sprites
    //Initialization
    //create sprite and load texture into unordered map
    //sf::Sprite hiddenTile(TextureManager::GetTexture("tile_hidden"));
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Minesweeper Works!");
    //pointer to window object for passing to other classes/methods
    sf::RenderWindow* window1 = &window;
    //create instance of Board object
    Board boardObj;
    bool lClick = false;
    //Event loop for real-time processing
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    lClick = true;
                }
                else
                    lClick = false;
                boardObj.boardClick(event.mouseButton.x, event.mouseButton.y, lClick);
            }
        }
        //always clear window before drawing
        window.clear();
        //draw background first
        boardObj.display(window1);
        //display all drawings
        window.display();
    }
    //clear textures before program ends
    TextureManager::Clear();

    return 0;
}