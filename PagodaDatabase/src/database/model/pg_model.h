#pragma once
#include "pgpch.h"

#include "common/pg_bina.h"
#include "common/pg_node_data.h"

namespace Pagoda::Database {
    struct VertexBufferElement {
        char* name;
        unsigned int type;
        unsigned int padding = 0;
    };

    struct ModelHeader {
        float* vertexBuffer;
        unsigned int* indexBuffer;
        VertexBufferElement** elementArray;
        unsigned int vertexBufferSize;
        unsigned int indexBufferSize;
        unsigned int elementCount;
        unsigned int padding1 = 0;
        unsigned int padding2 = 0;
        unsigned int padding3 = 0;
    };

    class ModelData : public NodeData {
    public:
        ModelData(std::vector<VertexBufferElement*> elements);
        virtual ~ModelData();

        static ModelData ModelDataFromNodeData(node_t* data);

        virtual void Print() const override;

    private:
        std::vector<VertexBufferElement*> m_vertexBufferElements;
    };
}