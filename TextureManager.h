//David Rowe
//Minesweeper Clone Project

#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
using std::unordered_map;
using std::string;

class TextureManager
{
	//static methods don't require an instance of the class with a object
	static unordered_map<string, sf::Texture> textures;
	static void LoadTexture(string textureName);
public:
	static sf::Texture& GetTexture(string textureName);						//GetTexture
	static void LoadTexture(string textureName, sf::IntRect size);			//overloaded for resizing feature sf::IntRec
	static sf::Texture& GetTexture(string textureName, sf::IntRect size);	//overloaded for resizing feature sf::IntRec
	static void Clear();													//Call once at end of main()

};

