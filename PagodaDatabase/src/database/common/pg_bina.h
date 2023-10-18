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

const unsigned int binaSig = 0x414E4942; // BINA
const unsigned int pacSig = 0x78434150; // PACx

const char binaVer[4] = "210";
const char dataSig[5] = "DATA";

const unsigned int additionalDataLength = 0x18;

namespace Pagoda::Database {
    class Node {
    public:
        static std::vector<unsigned long long> SeekOffsets(unsigned long long start, char* offsetTable, unsigned int offsetTableLength);
        static std::vector<unsigned long long> SeekOffsets(node_t* node);

        static std::vector<unsigned long long> PrintOffsets(char* offsetTable, unsigned int offsetTableLength, unsigned int start);
    };
}