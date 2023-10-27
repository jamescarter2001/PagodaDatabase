#pragma once
#include "pgpch.h"

#include "database/common/pg_bina.h"
#include "database/common/pg_node_data.h"

namespace Pagoda::Database {
    enum class LightFieldObjectType : uint8_t {
        E_SPHERE = 0,
        E_CAPSULE = 1,
        E_BOX = 2,
        E_OMNIBOX = 3
    };

    struct LightFieldObject {
        char* name;
        int unknown1;
        LightFieldObjectType objectType;
        uint8_t padding1;
        uint16_t unknown2;
        Vector5 scale;
        Vector3 translation;
        Vector4 rotation;
    };

    struct LightFieldAABBNode {
        int leftIndex;
        int rightIndex;
        Vector3 bBoxMin;
        Vector3 bBoxMax;
    };

    struct LightFieldHeader {
        unsigned int signature;
        unsigned int version;
        unsigned int unknown2;
        unsigned int objectCount;
        LightFieldObject* objectEntries;
        unsigned int nodeCount;
        LightFieldAABBNode* nodeEntries;
    };

    class LightFieldData : public NodeData {
    public:
        LightFieldData();
        LightFieldData(std::vector<LightFieldObject*> objectEntries, std::vector<LightFieldAABBNode*> nodeEntries);
        virtual ~LightFieldData();
        static LightFieldData LightFieldDataFromNodeData(data_t* data);
        virtual void Print() const override;

    private:
        std::string FormatObjectType(std::string name, LightFieldObjectType t) const;

        std::vector<LightFieldObject*> m_objectEntries;
        std::vector<LightFieldAABBNode*> m_nodeEntries;
    };
}