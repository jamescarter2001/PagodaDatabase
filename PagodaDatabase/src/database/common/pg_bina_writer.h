#pragma once
#include "database/common/pg_bina.h"

namespace Pagoda::Database {
    class BinaWriter {
    public:
        void AddStruct(void* structData, unsigned int size, bool isString = false);

        template<typename T>
        void AddStructVector(std::vector<T*>& structData, unsigned int size, bool isString = false) {
            this->AddStruct(&structData[0], sizeof(T*) * structData.size());

            for (T* s : structData) {
                this->AddStruct(s, size, isString);
            }
        }

        void Write(const char filePath[]);

        void WriteData(char** offset, std::vector<void*> structs);
        void FixPointers(char* nodeBody, std::vector<unsigned long long> offsets);

    private:
        std::vector<void*> m_structs;
        std::vector<void*> m_strings;
        std::vector<unsigned long long> m_offsets;

        std::map<void*, char*> m_offsetMap;
        std::map<void*, unsigned int> m_structSizeMap;

        unsigned int m_nodeSize = 0;
        unsigned int m_stringTableSize = 0;
    };
}