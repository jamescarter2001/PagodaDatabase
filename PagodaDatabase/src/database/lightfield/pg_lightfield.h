#pragma once
#include "pgpch.h"

#include "common/pg_bina.h"
#include "common/pg_node_data.h"

namespace Pagoda::Database {
    enum class LightFieldObjectType : uint8_t {
        E_SPHERE = 0,
        E_CAPSULE = 1,
        E_BOX = 2,
        E_OMNIBOX = 3
    };

    struct LightFieldObjectEntry {
        char* name;
        int unknown1;
        LightFieldObjectType objectType;
        uint8_t padding1;
        uint16_t unknown2;
        Vector5 scale;
        Vector3 translation;
        Vector4 rotation;
    };

    struct LightFieldBSPNodeEntry {
        int unknown1;
        int unknown2;
        Vector3 unknownVector1;
        Vector3 unknownVector2;
    };

    struct LightFieldHeader {
        unsigned int signature;
        unsigned int version;
        unsigned int unknown2;
        unsigned int objectCount;
        LightFieldObjectEntry* objectEntries;
        unsigned int nodeCount;
        LightFieldBSPNodeEntry* nodeEntries;
    };

    class LightFieldData : public NodeData {
    public:
        LightFieldData();
        LightFieldData(std::vector<LightFieldObjectEntry*> objectEntries, std::vector<LightFieldBSPNodeEntry*> nodeEntries);
        virtual ~LightFieldData();
        static LightFieldData LightFieldDataFromNodeData(data_t* data);
        virtual void Print() const override;

    private:
        std::string FormatObjectType(std::string name, LightFieldObjectType t) const;

        std::vector<LightFieldObjectEntry*> m_objectEntries;
        std::vector<LightFieldBSPNodeEntry*> m_nodeEntries;
    };
}