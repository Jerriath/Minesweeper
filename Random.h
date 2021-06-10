#pragma once
#include <random>

class Random
{
	static std::mt19937 random;
public:
	static int RandInt(int min, int max);
	static float RandFloat(float min, float mx);
};