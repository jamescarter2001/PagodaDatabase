#pragma once

#include "pgpch.h"

namespace Pagoda::Database {
    class NodeData {
    public:
        virtual void Print() const = 0;

    protected:
        static inline data_t* GetDataBlock(node_t* pNode) {
            return pNode + sizeof(NodeHeader);
        }
    };
}
