#pragma once
#include "pgpch.h"

#include "database/util/pg_database_util.h"

#include "database/common/pg_bina.h"
#include "database/common/pg_bina_raw.h"

#define BINA_SYM_U8 "u8"
#define BINA_SYM_U16 "u16"
#define BINA_SYM_U32 "u32"
#define BINA_SYM_U64 "u64"
#define BINA_SYM_S8 "s8"
#define BINA_SYM_S16 "s16"
#define BINA_SYM_S32 "s32"
#define BINA_SYM_S64 "s64"
#define BINA_SYM_F32 "f32"
#define BINA_SYM_F64 "f64"
#define BINA_SYM_STR "str"
#define BINA_SYM_REF "ref"

#define PTR_SIZE_32 4
#define PTR_SIZE_64 8

namespace Pagoda::Database {
    class BINATemplateConverter {
    public:
        BINATemplateConverter();
        BINATemplateConverter(unsigned int ptrSize);
        virtual ~BINATemplateConverter();

        void ConvertTemplateAndSave(const char src[], const char dest[], unsigned int ptrSize = 4, bool bigEndian = false);

    private:
        size_t GetSizeOfType(std::string type);

        void Write(char** ppDest, void* pSrc, std::string type);
        void WriteData(char** ppCurrentOffset, std::string type, std::string data, bool bigEndian = false);

        void InspectTemplate(const char src[]);

        unsigned int m_ptrSize = 0;

        std::stringstream m_offsetTable;
        std::map<std::string, unsigned int> m_offsetMap;
        std::vector<unsigned long long> m_offsets;

        std::stringstream m_stringTable;
        std::map<std::string, unsigned int> m_stringTablePositionMap;
        size_t m_dataBlockSize = 0;

        unsigned int m_stringTableOffset = 0;
    };
}