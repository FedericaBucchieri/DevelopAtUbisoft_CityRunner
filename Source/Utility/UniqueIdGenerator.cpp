#include "stdafx.h"
#include "UniqueIdGenerator.h"

int CUniqueIdGenerator::lastId = 0;
std::vector<int> CUniqueIdGenerator::recycledIds;