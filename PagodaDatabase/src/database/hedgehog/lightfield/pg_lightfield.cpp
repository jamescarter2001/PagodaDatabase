#include "pgpch.h"
#include "pg_lightfield.h"

namespace Pagoda::Database {
    LightFieldData::LightFieldData() {}

    LightFieldData::LightFieldData(std::vector<LightFieldObject*> objectEntries, std::vector<LightFieldAABBNode*> nodeEntries) : m_objectEntries(objectEntries), m_nodeEntries(nodeEntries) {}

    LightFieldData::~LightFieldData() {

    }

    std::string LightFieldData::FormatObjectType(std::string name, LightFieldObjectType t) const {
        std::string type;

        switch (t) {
            case LightFieldObjectType::E_SPHERE:
                type = "Sphere";
                break;
            case LightFieldObjectType::E_CAPSULE:
                type = "Capsule";
                break;
            case LightFieldObjectType::E_BOX:
                type = "Box";
                break;
            case LightFieldObjectType::E_OMNIBOX:
                type = "OmniBox";
        }

        return std::string(this->FormatName(name) + type);
    }

    LightFieldData LightFieldData::LightFieldDataFromNodeData(data_t* data) {
        LightFieldHeader* header = (LightFieldHeader*)data;

        std::vector<LightFieldObject*> objectEntries;
        for (unsigned int i = 0; i < header->objectCount; i++) {
            LightFieldObject* e = header->objectEntries + i;
            objectEntries.push_back(e);
        }

        std::vector<LightFieldAABBNode*> nodeEntries;
        for (unsigned int i = 0; i < header->nodeCount; i++) {
            LightFieldAABBNode* e = header->nodeEntries + i;
            nodeEntries.push_back(e);
        }

        return LightFieldData(objectEntries, nodeEntries);
    }

    void LightFieldData::Print() const {
        unsigned int warnings = 0;



        std::cout << "" << std::endl;
        std::cout << "LIGHTFIELD DATA" << std::endl;
        std::cout << "----------" << std::endl;
        for (LightFieldObject* e : this->m_objectEntries) {
            std::cout << this->FormatString("Name", e->name) << std::endl;
            std::cout << this->FormatInt("Unknown Signed Integer", e->unknown1) << std::endl;
            std::cout << this->FormatObjectType("Type", e->objectType) << std::endl;
            // std::cout << this->FormatUnsignedInt("Padding", e->padding1) << std::endl;
            std::cout << this->FormatUnsignedInt("Unknown Integer 16", e->unknown2) << std::endl;
            std::cout << this->FormatVector5("Scale", e->scale) << std::endl;
            std::cout << this->FormatVector3("Translation", e->translation) << std::endl;
            std::cout << this->FormatVector4("Rotation", e->rotation) << std::endl;
            std::cout << "----------" << std::endl;

            this->Validate(e->name, e->padding1 == 0, "Padding is not zero", &warnings);
        }

        int highestInt1 = 0;
        int highestInt2 = 0;

        std::cout << "LIGHTFIELD DATA [NODE]" << std::endl;
        std::cout << "----------" << std::endl;
        for (LightFieldAABBNode* e : this->m_nodeEntries) {
            std::cout << this->FormatInt("Unknown Signed Integer (1)", e->leftIndex) << std::endl;
            std::cout << this->FormatInt("Unknown Signed Integer (2)", e->rightIndex) << std::endl;
            std::cout << this->FormatVector3("Unknown Vector 3 (1)", e->bBoxMin) << std::endl;
            std::cout << this->FormatVector3("Unknown Vector 3 (2)", e->bBoxMax) << std::endl;
            std::cout << "----------" << std::endl;

            if (e->leftIndex > highestInt1) {
                highestInt1 = e->leftIndex;
            }

            if (e->rightIndex > highestInt2) {
                highestInt2 = e->rightIndex;
            }
        }

        std::cout << this->FormatUnsignedInt("Object Count", this->m_objectEntries.size()) << std::endl;
        std::cout << this->FormatUnsignedInt("Node Count", this->m_nodeEntries.size()) << std::endl;
        std::cout << this->FormatInt("Highest Signed Integer 1", highestInt1) << std::endl;
        std::cout << this->FormatInt("Highest Signed Integer 2", highestInt2) << std::endl;
        std::cout << "----------" << std::endl;
        std::cout << this->FormatUnsignedInt("Warnings", warnings) << std::endl;
    }
}