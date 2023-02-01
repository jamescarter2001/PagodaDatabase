#include "pgpch.h"

#include "pg_bina.h"

namespace Pagoda::Database {
    std::vector<unsigned long> Node::SeekOffsets(unsigned long start) {
        std::vector<unsigned long> offsets;

        char* offsetTable = this->nodeBody->offsetTable;

        unsigned long currentOffset = start + sizeof(this->nodeHeader) + nodeHeader.additionalDataLength;
        while (*offsetTable != '\0') {
            if (*offsetTable & BINA_OFFSET_6) {
                unsigned char offset;
                memcpy(&offset, offsetTable, 1);

                offset = offset << 2;

                currentOffset = currentOffset + offset;
                offsets.push_back(currentOffset);
                offsetTable++;
            } else if (*offsetTable & BINA_OFFSET_14) {
                unsigned short offset;
                memcpy(&offset, offsetTable, 2);

                offset = offset << 2;

                offset = _byteswap_ushort(offset);
                currentOffset = currentOffset + offset;

                offsets.push_back(currentOffset);
                offsetTable += 2;
            } else if (*offsetTable & BINA_OFFSET_30) {
                unsigned long offset;
                memcpy(&offset, offsetTable, 4);

                offset = offset << 2;

                offset = _byteswap_ulong(offset);
                currentOffset = currentOffset + offset;

                offsets.push_back(currentOffset);
                offsetTable += 4;
            }

            //std::cout << "0x" << std::hex << currentOffset << std::endl;
        }

        return offsets;
    }

    std::vector<unsigned long long> Node::SeekOffsets(node_t* node) {
        NodeHeader nh = *(NodeHeader*)node;
        char* offsetTable = node + sizeof(nh) + nh.additionalDataLength + nh.stringTableOffset + nh.stringTableLength;

        unsigned long long current = (unsigned long long) node + sizeof(nh) + nh.additionalDataLength;

        std::vector<unsigned long long> offsets;
        while (*offsetTable != '\0') {
            if (*offsetTable & BINA_OFFSET_6) {
                unsigned long o = 0;

                unsigned char offset;
                memcpy(&offset, offsetTable, sizeof(char));

                offset = offset << 2;
                o = o | offset;

                current = current + o;

                offsets.push_back(current);

                offsetTable++;
            } else if (*offsetTable & BINA_OFFSET_14) {
                unsigned long o = 0;

                unsigned char offset[2];
                memcpy(&offset, offsetTable, sizeof(char) * 2);

                for (int i = 0; i < sizeof(offset); i++) {
                    offset[i] = offset[i] << 2;
                    o = o | offset[i];
                }

                current = current + o;

                offsets.push_back(current);
                offsetTable = offsetTable + 2;
            } else if (*offsetTable & BINA_OFFSET_30) {
                unsigned long o = 0;

                unsigned char offset[4];
                memcpy(&offset, offsetTable, sizeof(char) * 4);

                for (int i = 0; i < sizeof(offset); i++) {
                    offset[i] = offset[i] << 2;
                    o = o | offset[i];
                }

                current = current + o;

                offsets.push_back(current);
                offsetTable = offsetTable + 4;
            }
        }

        return offsets;
    }
}