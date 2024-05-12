#include "stdafx.h"
#include "Random.h"
#include <random>

float CRandom::GetRandomFloatInRange(float min, float max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> random(min, max);
	return  random(gen);
}

int CRandom::GetRandomIntInRange(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> random(min, max);
	return  random(gen);
}

bool CRandom::GetRandomBool()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> random(0, 1);
	return  random(gen) == 1;
}
