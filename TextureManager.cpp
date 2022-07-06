//David Rowe
//Minesweeper Clone Project

#include "TextureManager.h"

//redeclare variable so this file knows it exists
unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(string fileName)
{
	//create file path
	string path = "images/";
	path += fileName + ".png";
	//load texture image into unordered map
	textures[fileName].loadFromFile(path);
}

//overloaded for sf::IntRect to resize textures
void TextureManager::LoadTexture(string fileName, sf::IntRect size)
{
	//create file path
	string path = "images/";
	path += fileName + ".png";
	//load texture image into unordered map
	textures[fileName].loadFromFile(path, size);
}

//retrieve texture image from unordered map (textures)
sf::Texture& TextureManager::GetTexture(string texturename)
{
	//if texture does not exist... load first then return
	if (textures.find(texturename) == textures.end())
	{
		LoadTexture(texturename);
	}
	return textures[texturename];
}

//overloaded for sf::IntRect to resize textures
//retrieve texture image from unordered map (textures)
sf::Texture& TextureManager::GetTexture(string texturename, sf::IntRect size)
{
	//if texture does not exist... load first then return
	if (textures.find(texturename) == textures.end())
	{
		LoadTexture(texturename, size);
	}
	return textures[texturename];
}

void TextureManager::Clear()
{
	//clear texture images from unordered map
	textures.clear();
}