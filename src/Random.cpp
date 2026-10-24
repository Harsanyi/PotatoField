#include "Random.h"
#include <stdexcept>

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
	if (max <= min) throw std::invalid_argument("Invalid integer random range");
	return std::uniform_int_distribution<int>(min, max - 1)(generator);
}

std::size_t Random::getIndex(std::size_t count)
{
    if (!count) throw std::out_of_range("Cannot choose from an empty range");
    return std::uniform_int_distribution<std::size_t>(0, count - 1)(generator);
}
