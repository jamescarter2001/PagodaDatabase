#include "pgpch.h"

#include "pg_bina.h"

namespace Pagoda::Database {
    std::vector<unsigned long long> Node::SeekOffsets(node_t* node) {
        NodeHeader nh = *(NodeHeader*)node;
        char* offsetTable = node + sizeof(nh) + nh.additionalDataLength + nh.stringTableOffset + nh.stringTableLength;

        unsigned long long current = (unsigned long long) node + sizeof(nh) + nh.additionalDataLength;

        std::vector<unsigned long long> offsets;
        while (*offsetTable != '\0') {
            if (*offsetTable & BINA_OFFSET_6) {
                unsigned long o = 0;

                uint8_t* dat = (uint8_t*)offsetTable;
                o = *dat - 0x40;
                o = o << 2;

                current = current + o;

                offsets.push_back(current);
                offsetTable++;
            } else if (*offsetTable & BINA_OFFSET_14) {
                unsigned long o = 0;

                uint16_t* dat = (uint16_t*)offsetTable;
                o = _byteswap_ushort(*dat);
                o = o - 0x8000;
                o = o << 2;

                current = current + o;

                offsets.push_back(current);
                offsetTable = offsetTable + 2;
            } else if (*offsetTable & BINA_OFFSET_30) {
                unsigned long o = 0;

                uint32_t* dat = (uint32_t*)offsetTable;
                o = _byteswap_ulong(*dat);
                o = o - 0xC0000000;
                o = o << 2;

                current = current + o;

                offsets.push_back(current);
                offsetTable = offsetTable + 4;
            } else {
                std::cout << "Invalid offset table!" << std::endl;
                return std::vector<unsigned long long>();
            }
        }
        return offsets;
    }
}