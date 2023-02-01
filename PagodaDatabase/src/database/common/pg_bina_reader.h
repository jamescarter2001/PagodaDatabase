#pragma once
#include "btpch.h"

#include "pg_bina.h"

#define READ_BYTES(f, s, n) f.read((char*)s, n)
#define READ_STRUCT(f, s) f.read((char*)&s, sizeof(s))

namespace Pagoda::Database {
    class BinaReader {
    public:
        BinaReader();
        virtual ~BinaReader();

        Bina ReadLegacy(std::string path);
        std::vector<data_t*> Read(std::string path);

    private:
        void ResolveOffsets(node_t* n);
    };
}