//David Rowe
//Minesweeper Clone Project
#pragma once
#include <random>
class Random
{
	static std::mt19937 random;
public:
	//static methods don't require an instance of the class with a object
	static int Int(int min, int max);
	static int Float(float min, float max);
};

