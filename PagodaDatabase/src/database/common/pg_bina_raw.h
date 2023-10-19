#pragma once

#define ADD_DATA_LENGTH 0x18

namespace Pagoda::Database {

    // PACx2
    
    struct PACV2Header {
        unsigned int signature;
        unsigned int blockSize;
        unsigned int dataEntriesSize;
        unsigned int proxiesSize;
        unsigned int stringTableSize;
        unsigned int offsetTableSize;
        uint8_t unknown1;
        uint8_t padding1;
        uint16_t padding2;
    };

    // BINA1

    struct BINAV1Header {
        unsigned int fileSize;
        unsigned int offsetTableOffset;
        unsigned int offsetTableLength;
        unsigned int padding;

        unsigned short unknownFlag1;
        unsigned short footerMagicPresent;
        char reservedValues[2];
        unsigned int signature;
        unsigned int padding2;
    };

    // BINA2

    struct BINAHeader {
        unsigned int header;
        char version[3];
        char endianFlag;
        unsigned int fileSize;
        short nodeCount;
        short Unknown1;
    };

    struct PACProxyTableEntry {
        char* type;
        char* name;
        unsigned int nodeIndex;
    };

    struct PACProxyTable {
        unsigned int proxyCount;
        PACProxyTableEntry** entries;
    };

    struct NodeHeader {
        unsigned int signature;
        unsigned int length;

        unsigned int stringTableOffset;
        unsigned int stringTableLength;

        unsigned int offsetTableLength;
        short additionalDataLength = ADD_DATA_LENGTH;
        short padding;
        char additionalData[ADD_DATA_LENGTH];
    };
}