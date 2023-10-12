#pragma once
#include "database/common/pg_bina.h"

namespace Pagoda::Database {
    class BinaWriter {
    public:
        void AddStruct(void* structData, size_t size, bool isString = false);

        template<typename T>
        void AddStructVector(std::vector<T*>& structData, bool isString = false) {
            this->AddStruct(&structData[0], sizeof(T*) * (unsigned int)structData.size());

            for (T* s : structData) {
                this->AddStruct(s, sizeof(T), isString);
            }
        }

        void Write(const char filePath[]);

        void WriteData(char** offset, std::vector<void*> structs);
        void FixPointers(char* nodeBody, std::vector<unsigned long long> offsets);
        unsigned int GetAlignment(unsigned int count);

    private:
        std::vector<void*> m_structs;
        std::vector<void*> m_strings;
        std::vector<unsigned long long> m_offsets;

        std::map<void*, char*> m_offsetMap;
        std::map<void*, size_t> m_structSizeMap;

        size_t m_nodeSize = 0;
        size_t m_stringTableSize = 0;
    };
}