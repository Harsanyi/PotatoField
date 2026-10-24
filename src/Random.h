#pragma once

#include "random"

class Random
{
	static std::random_device device;
	static std::mt19937_64 generator;
	static std::uniform_real_distribution<double> distributor;

	Random();

public:
	static double getDouble();
	static double getDouble(double min, double max);
	static int getInt(int min, int max);
};

