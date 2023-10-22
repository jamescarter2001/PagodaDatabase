#include "pgpch.h"
#include "pg_node_data.h"

namespace Pagoda::Database {
    void NodeData::Validate(const char* name, bool condition, std::string msg, unsigned int* count) const {
        if (!condition) {
            std::cout << "WARNING: " + std::string(name) + " " + msg << std::endl;
            if (count != nullptr) {
                *count = *count + 1;
            }
        }
    }

    std::string NodeData::FormatName(std::string name) const {
        return std::string(name + ": ");
    }

    std::string NodeData::FormatString(std::string name, char* s) const {
        return std::string(this->FormatName(name) + std::string(s));
    }

    std::string NodeData::FormatInt(std::string name, int i) const {
        return std::string(this->FormatName(name) + std::to_string(i));
    }

    std::string NodeData::FormatUnsignedInt(std::string name, unsigned int i) const {
        return std::string(this->FormatName(name) + std::to_string(i));
    }

    std::string NodeData::FormatVector3(std::string name, Vector3 v) const {
        return std::string(this->FormatName(name) + std::to_string(v.X) + " " + std::to_string(v.Y) + " " + std::to_string(v.Z));
    }

    std::string NodeData::FormatVector4(std::string name, Vector4 v) const {
        return std::string(this->FormatName(name) + std::to_string(v.X) + " " + std::to_string(v.Y) + " " + std::to_string(v.Z) + " " + std::to_string(v.W));
    }

    std::string NodeData::FormatVector5(std::string name, Vector5 v) const {
        return std::string(this->FormatName(name) + std::to_string(v.X) + " " + std::to_string(v.Y) + " " + std::to_string(v.Z) + " " + std::to_string(v.W) + " " + std::to_string(v.A));
    }
}