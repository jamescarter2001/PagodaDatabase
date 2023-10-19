#include "pgpch.h"
#include "pg_model.h"

namespace Pagoda::Database {
    ModelData::ModelData(std::vector<VertexBufferElement*> elements) : m_vertexBufferElements(elements) {
    }

    ModelData::~ModelData() {
    }

    ModelData ModelData::ModelDataFromNodeData(data_t* data) {
        ModelHeader* header = (ModelHeader*)data;

        std::vector<VertexBufferElement*> elements;
        for (unsigned int i = 0; i < header->elementCount; i++) {
            VertexBufferElement* element = *(header->elementArray + i);

            elements.push_back(element);
        }

        return ModelData::ModelData(elements);
    }

    void ModelData::Print() const {
        std::cout << "MODEL DATA" << std::endl;
        std::cout << "----------" << std::endl;
        for (VertexBufferElement* e : this->m_vertexBufferElements) {
            std::cout << "Name: " << e->name << std::endl;
            std::cout << "Type: " << e->type << std::endl;
            std::cout << "----------" << std::endl;
        }
    }
}