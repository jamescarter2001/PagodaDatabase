#pragma once

#include "pgpch.h"

#include "common/pg_bina.h"
#include "common/pg_node_data.h"

#include "pg_tag.h"

#define TAG_TYPE_RANGE_SPAWNING "RangeSpawning"

namespace Pagoda::Database {
    struct RangersUUID {
        unsigned int X, Y, Z, W;
    };

    struct ForcesObjectReference {
        unsigned short ID;
        unsigned short GroupID;
    };

    struct ObjectEntry {
        unsigned long long Padding1;
        char* ObjectType;
        char* ObjectName;
        ForcesObjectReference ID;
        ForcesObjectReference ParentID;
        RangersUUID UUID;
        RangersUUID ParentUUID;
        Vector3 Position;
        Vector3 Rotation;
        Vector3 ChildPositionOffset;
        Vector3 ChildRotationOffset;
        Tag** TagOffsetTable;
        unsigned long long TagCount;
        unsigned long long TagCount2;
        unsigned long long Padding3;
        void* Parameters;
    };

    struct GEditHeader {
        unsigned long long Padding1;
        unsigned long long Padding2;
        ObjectEntry** Objects;
        unsigned long long ObjectCount;
        unsigned long long ObjectCount2;
        unsigned long long Padding3;
    };

    class SetData : public NodeData {
    public:
        SetData();
        SetData(std::vector<ObjectEntry*> objectEntries);
        virtual ~SetData();
        static SetData SetDataFromNodeData(data_t* data);
        virtual void Print() const override;

        static void PrintTag(Tag* tag);
    private:
        std::vector<ObjectEntry*> m_ObjectEntries;
    };
}