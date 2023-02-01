#pragma once

#include "pgpch.h"

namespace Pagoda::Database {
    class NodeData {
    public:
        virtual void Print() const = 0;
    };
}
