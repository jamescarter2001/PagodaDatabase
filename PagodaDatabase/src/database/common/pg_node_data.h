#pragma once

#include "pgpch.h"

#include "database/common/pg_bina.h"
#include "database/common/pg_bina_raw.h"

namespace Pagoda::Database {
    class NodeData {
    public:
        virtual void Print() const = 0;
    };
}
