#pragma once

//
// CRandom is a utility class for generating random numbers.
//

class CRandom
{

public:
	static float GetRandomFloatInRange(float min, float max);
	static int GetRandomIntInRange(int min, int max);
	static bool GetRandomBool();

};
