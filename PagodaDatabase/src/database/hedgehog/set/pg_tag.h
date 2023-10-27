#pragma once
#include "pgpch.h"

namespace Pagoda::Database {
    struct Tag {
        unsigned long long Padding1;
        char* TagType;
        unsigned long long DataLength;
    };

    struct RangeSpawningData {
        float RangeIn;
        float RangeOut;
    };

    struct RangeSpawning : public Tag {
        RangeSpawningData* Data;
    };
}