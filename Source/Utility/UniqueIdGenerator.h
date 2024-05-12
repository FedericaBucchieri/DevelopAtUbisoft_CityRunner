#pragma once
#include "stdafx.h"

//
// CUniqueIdGenerator is a utility class for generating and recycling unique IDs.
//

class CUniqueIdGenerator {
public:
    static int GetNextId() {
        if (recycledIds.empty()) {
            return ++lastId;
        }
        else {
            int recycledId = recycledIds.back();
            recycledIds.pop_back();
            return recycledId;
        }
    }

    static void RecycleId(int id) {
        recycledIds.push_back(id);
    }

private:
    static int lastId;
    static std::vector<int> recycledIds;
};

