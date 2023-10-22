#pragma once

#include "pgpch.h"

#include "database/common/pg_bina.h"
#include "database/common/pg_bina_raw.h"

namespace Pagoda::Database {
    class NodeData {
    public:
        virtual void Print() const = 0;

    protected:
        void Validate(const char* name, bool condition, std::string msg, unsigned int* count = nullptr) const;
        std::string FormatString(std::string name, char* s) const;
        std::string FormatInt(std::string name, int i) const;
        std::string FormatUnsignedInt(std::string name, unsigned int i) const;
        std::string FormatVector3(std::string name, Vector3 v) const;
        std::string FormatVector4(std::string name, Vector4 v) const;
        std::string FormatVector5(std::string name, Vector5 v) const;
        std::string FormatName(std::string name) const;
    };
}
