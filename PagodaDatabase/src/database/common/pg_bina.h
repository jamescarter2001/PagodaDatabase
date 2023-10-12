#pragma once
#include "pgpch.h"

#include "pg_bina_raw.h"

typedef char bina_t;
typedef char node_t;
typedef char data_t;

#define BINA_OFFSET_END 0b00000000
#define BINA_OFFSET_6 0b01000000
#define BINA_OFFSET_14 0b10000000
#define BINA_OFFSET_30 0b11000000

const char binaSig[5] = "BINA";
const char binaVer[4] = "100";
const char dataSig[5] = "DATA";

const unsigned int additionalDataLength = 0x18;

namespace Pagoda::Database {
    class Node {
    public:
        static std::vector<unsigned long long> SeekOffsets(node_t* node);
    };
}