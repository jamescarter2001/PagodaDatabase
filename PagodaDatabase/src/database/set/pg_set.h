#pragma once

#include "btpch.h"

#include "common/pg_bina.h"
#include "common/pg_node_data.h"

#define TAG_TYPE_RANGE_SPAWNING "RangeSpawning"

namespace Pagoda::Database {
    struct Vector3 {
        float X, Y, Z;
    };

    struct Vector4 {
        float X, Y, Z, W;
    };

    struct ForcesObjectReference {
        unsigned short ID;
        unsigned short GroupID;
    };

    struct RangersUnknown {
        Vector3 Position;
        Vector3 Rotation;
        Vector3 ChildPositionOffset;
        Vector3 ChildRotationOffset;
    };

    struct Tag {
        unsigned long long Padding1;
        char* TagType;
        unsigned long long DataLength;
    };

    struct RangeSpawningData {
        float RangeIn;
        float RangeOut;
    };

    struct RangeSpawning : public Tag {
        RangeSpawningData* Data;
    };

    struct ObjectEntry {
        unsigned long long Padding1;
        char* ObjectType;
        char* ObjectName;
        ForcesObjectReference ID;
        ForcesObjectReference ParentID;
        Vector4 UnknownVector1;
        Vector4 UnknownVector2;
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
        static SetData SetDataFromNodeData(data_t* data);
        virtual ~SetData();
        virtual void Print() const;

        static void PrintTag(Tag* tag);
    private:
        std::vector<ObjectEntry*> m_ObjectEntries;
    };
}