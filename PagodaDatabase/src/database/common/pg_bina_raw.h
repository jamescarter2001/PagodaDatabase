#pragma once

namespace Pagoda::Database {
    struct Header {
        char header[4];
        char version[3];
        char endianFlag;
        unsigned int fileSize;
        short nodeCount;
        short Unknown1;
    };

    struct NodeHeader {
        char signature[4];
        int length;  // Only true if the DATA node is the only node in the file, however this is always the case from our findings.

        unsigned int stringTableOffset;  // The non-absolute (relative to the beginning of the Data array below, which is always 0x40 from our findings) offset to the BINA String Table.
        unsigned int stringTableLength;  // The length of the BINA String Table.

        unsigned int offsetTableLength;  // The length of the BINA Offset Table.
        short additionalDataLength;
        short padding;  // Just two nulls to pad-out AdditionalDataLength to 4 bytes
    };

    struct NodeBody {
        NodeBody(NodeHeader nodeHeader) {
            this->additionalData = new char[nodeHeader.additionalDataLength];
            this->dataBlock = new char[nodeHeader.stringTableOffset];
            this->stringTable = new char[nodeHeader.stringTableLength];
            this->offsetTable = new char[nodeHeader.offsetTableLength];
        }

        virtual ~NodeBody() {
            delete additionalData;
            delete dataBlock;
            delete stringTable;
            delete offsetTable;
        }

        char* additionalData;
        char* dataBlock;
        char* stringTable;
        char* offsetTable;
    };
}