#include "pgpch.h"
#include "pg_bina_writer.h"

namespace Pagoda::Database {
    void BinaWriter::AddStruct(void* structData, size_t size) {
        // Store the size of the struct.
        this->m_structSizeMap.insert({structData, size});

        this->m_structs.push_back(structData);
        this->m_structSize += size;
    }

    void BinaWriter::AddString(char* str) {
        // Get string size, accounting for null termination.
        size_t size = strlen(str) + 1;

        this->m_structSizeMap.insert({str, size});

        this->m_strings.push_back(str);
        this->m_stringTableSize += size;
    }
    
    void BinaWriter::WriteData(char** offset, std::vector<void*> structs) {
        for (void* structData : structs) {
            unsigned long long structSize = this->m_structSizeMap.find(structData)->second;
            memcpy(*offset, structData, structSize);
            this->m_offsetMap.insert({structData, *offset});
            *offset += structSize;
        }

        unsigned long long alignment = (unsigned long long)*offset % 4;

        if (alignment != 0) {
            *offset += 4 - alignment;
        }
    }

    void BinaWriter::Write(const char filePath[]) {
        unsigned int nodeAlignment = GetAlignment(this->m_structSize);
        unsigned int stringAlignment = GetAlignment(this->m_stringTableSize);

        // Allocate enough bytes for bina header and node data.
        unsigned long long heapSize = sizeof(Header) + sizeof(NodeHeader) + this->m_structSize + this->m_stringTableSize + nodeAlignment + stringAlignment;
        char* pBinaNode = new char[heapSize];

        // Initialise the allocated heap space to zero.
        memset(pBinaNode, 0, heapSize);

        std::ofstream outFile;
        outFile.open(filePath, std::ios::out | std::ios::binary);

        Header* binaHeader = (Header*)pBinaNode;
        NodeHeader* nodeHeader = (NodeHeader*)(pBinaNode + sizeof(Header));

        // Set header information.
        binaHeader->header = binaSig;
        memcpy(binaHeader->version, binaVer, 3);
        memcpy(nodeHeader->signature, dataSig, 4);
        binaHeader->endianFlag = 'L';

        binaHeader->nodeCount = 1;

        char* pCurrentOffset = (char*)nodeHeader + sizeof(NodeHeader);

        char* pNodeBody = pCurrentOffset;

        // Write data and string table.
        WriteData(&pCurrentOffset, this->m_structs);
        WriteData(&pCurrentOffset, this->m_strings);

        // Set string table information.
        nodeHeader->stringTableOffset = (unsigned int)this->m_structSize + nodeAlignment;
        nodeHeader->stringTableLength = (unsigned int)this->m_stringTableSize + stringAlignment;

        // Map pointers to the correct structs in the file.
        FixPointers(pNodeBody);

        unsigned int offsetTableSize = 0;

        // Write offset table.
        std::stringstream offsetTableStream;
        for (unsigned long long o : this->m_offsets) {
            if (o > 0xFFFC) {
                uint32_t val = ((uint32_t)o >> 2) | 0xC0000000;
                char* bytes = (char*)&val;
                offsetTableStream << bytes[3] << bytes[2] << bytes[1] << bytes[0];
                offsetTableSize+=4;
            } else if (o > 0xFC) {
                uint16_t val = ((uint16_t)o >> 2) | 0x8000;
                char* bytes = (char*)&val;
                offsetTableStream << bytes[1] << bytes[0];
                offsetTableSize+=2;
            } else {
                uint8_t val = ((uint8_t)o >> 2) | 0x40;
                offsetTableStream << val;
                offsetTableSize++;
            }
        }

        // Account for null at end of offset table.
        offsetTableSize++;

        unsigned int offsetTableAlignment = GetAlignment(offsetTableSize);

        // Set BINA and node size, then write the file.
        nodeHeader->offsetTableLength = offsetTableSize + offsetTableAlignment;
        nodeHeader->length = sizeof(NodeHeader) + (unsigned int)this->m_structSize + nodeAlignment + (unsigned int)this->m_stringTableSize + stringAlignment + offsetTableSize + offsetTableAlignment;
        binaHeader->fileSize = sizeof(Header) + (size_t)nodeHeader->length;
        outFile.write(pBinaNode, binaHeader->fileSize - offsetTableSize - offsetTableAlignment);

        outFile << offsetTableStream.str();

        for (unsigned int i = 0; i < (offsetTableAlignment + 1); i++) {
            outFile << '\0';
        }

        outFile.close();

        delete[] pBinaNode;
    }

    unsigned int BinaWriter::GetAlignment(size_t count) {
        size_t val = 4 - (count % 4);
        if (val == 4) {
            return 0;
        } else {
            return (unsigned int)val;
        }
    }

    void BinaWriter::FixPointers(char* nodeBody) {
        unsigned int count = (unsigned int)(this->m_structSize / sizeof(void*));
        char** ptr = (char**)nodeBody;

        unsigned long long lastOffset = 0;
        for (unsigned int i = 0; i < count; i++) {
            auto relativeOffset = this->m_offsetMap.find(*(ptr + i));
            if (relativeOffset != this->m_offsetMap.end()) {
                *(ptr + i) = (char*)(relativeOffset->second - nodeBody);

                unsigned long long offset = (unsigned long long)(ptr + i) - (unsigned long long)nodeBody;
                this->m_offsets.push_back(offset - lastOffset);

                lastOffset = offset;
            }
        }
    }
}