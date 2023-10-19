#pragma once
#include "pgpch.h"

#include "pg_bina.h"

#define READ_BYTES(f, s, n) f.read((char*)s, n)
#define READ_STRUCT(f, s) f.read((char*)&s, sizeof(s))

namespace Pagoda::Database {
    class BinaReader {
    public:
        BinaReader();
        virtual ~BinaReader();

        std::vector<node_t*> Read(std::string path);
    };
}