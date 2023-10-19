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

    void BinaWriter::AddStringVector(std::vector<char*>& structData) {
        this->AddStruct(&structData[0], sizeof(char*) * (unsigned int)structData.size());

        for (char* s : structData) {
            this->AddString(s);
        }
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
        unsigned int stringAlignment = GetAlignment(this->m_stringTableSize);

        // Allocate enough bytes for bina header and node data.
        unsigned long long heapSize = sizeof(BINAHeader) + sizeof(NodeHeader) + this->m_structSize + this->m_stringTableSize + stringAlignment;
        char* pBinaNode = new char[heapSize];

        // Initialise the allocated heap space to zero.
        memset(pBinaNode, 0, heapSize);

        std::ofstream outFile;
        outFile.open(filePath, std::ios::out | std::ios::binary);

        BINAHeader* binaHeader = (BINAHeader*)pBinaNode;
        NodeHeader* nodeHeader = (NodeHeader*)(pBinaNode + sizeof(BINAHeader));

        // Set header information.
        binaHeader->header = binaSig;
        memcpy(binaHeader->version, binaVer, 3);
        nodeHeader->signature = dataSig;
        binaHeader->endianFlag = 'L';

        binaHeader->nodeCount = 1;

        char* pCurrentOffset = (char*)nodeHeader + sizeof(NodeHeader);

        char* pNodeBody = pCurrentOffset;

        // Write data and string table.
        WriteData(&pCurrentOffset, this->m_structs);
        WriteData(&pCurrentOffset, this->m_strings);

        // Set string table information.
        nodeHeader->stringTableOffset = (unsigned int)this->m_structSize;
        nodeHeader->stringTableLength = (unsigned int)this->m_stringTableSize + stringAlignment;

        // Map pointers to the correct structs in the file.
        FixPointers(pNodeBody);

        std::stringstream offsetTableStream = DatabaseUtils::GenerateBINAOffsetTable(this->m_offsets);
        DatabaseUtils::Align(offsetTableStream);

        // Set BINA and node size, then write the file.
        nodeHeader->offsetTableLength = (unsigned int)offsetTableStream.str().size();
        nodeHeader->length = sizeof(NodeHeader) + (unsigned int)this->m_structSize + (unsigned int)this->m_stringTableSize + stringAlignment + nodeHeader->offsetTableLength;
        binaHeader->fileSize = sizeof(BINAHeader) + (size_t)nodeHeader->length;
        outFile.write(pBinaNode, binaHeader->fileSize - nodeHeader->offsetTableLength);

        outFile << offsetTableStream.str();

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