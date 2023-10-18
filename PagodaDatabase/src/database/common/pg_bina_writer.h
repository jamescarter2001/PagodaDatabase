#pragma once
#include "database/common/pg_bina.h"

namespace Pagoda::Database {
    class BinaWriter {
    public:
        void AddString(char* str);
        void AddStruct(void* structData, size_t size);

        void AddStringVector(std::vector<char*>& structData) {
            this->AddStruct(&structData[0], sizeof(char*) * (unsigned int)structData.size());

            for (char* s : structData) {
                this->AddString(s);
            }
        }

        template<typename T>
        void AddStructVector(std::vector<T*>& structData) {
            this->AddStruct(&structData[0], sizeof(T*) * (unsigned int)structData.size());

            for (T* s : structData) {
                this->AddStruct(s, sizeof(T));
            }
        }

        void Write(const char filePath[]);

        void WriteData(char** offset, std::vector<void*> structs);
        void FixPointers(char* nodeBody);
        unsigned int GetAlignment(size_t count);

    private:
        std::vector<void*> m_structs;
        std::vector<void*> m_strings;
        std::vector<unsigned long long> m_offsets;

        std::map<void*, char*> m_offsetMap;
        std::map<void*, size_t> m_structSizeMap;

        size_t m_structSize = 0;
        size_t m_stringTableSize = 0;
    };
}