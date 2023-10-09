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