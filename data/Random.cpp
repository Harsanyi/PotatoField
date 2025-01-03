#include "Random.h"

std::random_device Random::device{};
std::mt19937_64 Random::generator{device()};
std::uniform_real_distribution<double> Random::distributor{0.,1.};

double Random::getDouble()
{
	return distributor(generator);
}

double Random::getDouble(double min, double max)
{
	return getDouble() * (max - min) + min;
}

int Random::getInt(int min, int max)
{
	return (int)getDouble(min,max);
}
