#include "Random.h"
#include <ctime>

std::mt19937 Random::random(time(0));

float Random::RandFloat(float min, float max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(random);
}
int Random::RandInt(int min, int max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(random);
}