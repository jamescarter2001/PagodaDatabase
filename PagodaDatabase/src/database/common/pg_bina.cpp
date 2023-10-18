#include "pgpch.h"

#include "pg_bina.h"

namespace Pagoda::Database {
    std::vector<unsigned long long> Node::SeekOffsets(unsigned long long start, char* offsetTable, unsigned int offsetTableLength) {
        unsigned long long current = start;

        std::vector<unsigned long long> offsets;
        for (unsigned int i = 0; i < offsetTableLength;) {
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
            } else if (*offsetTable == '\0') {
                // End of BINA200 offset table.
                return offsets;
            } else {
                std::cout << "Invalid offset table!" << std::endl;
                return std::vector<unsigned long long>();
            }
        }
        return offsets;
    }

    std::vector<unsigned long long> Node::PrintOffsets(char* offsetTable, unsigned int offsetTableLength, unsigned int start) {
        std::vector<unsigned long long> offsets = Node::SeekOffsets(start, offsetTable, offsetTableLength);

        for (unsigned long long o : offsets) {
            std::cout << std::hex << o << std::endl;
        }

        return offsets;
    }

    std::vector<unsigned long long> Node::SeekOffsets(node_t* node) {
        NodeHeader nh = *(NodeHeader*)node;

        char* offsetTable = node + sizeof(nh) + nh.stringTableOffset + nh.stringTableLength;
        unsigned long long start = (unsigned long long) node + sizeof(nh);

        return Node::SeekOffsets(start, offsetTable, nh.offsetTableLength);
    }
}