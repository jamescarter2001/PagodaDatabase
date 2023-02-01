#include "pgpch.h"

#include "pg_set.h"

namespace Pagoda::Database {

 SetData::SetData() {

}

 SetData::SetData(std::vector<ObjectEntry*> objectEntries) {
    this->m_ObjectEntries = objectEntries;
 }

 
 SetData SetData::SetDataFromNodeData(data_t* data) {
    GEditHeader* header = (GEditHeader*) data;

    std::vector<ObjectEntry*> objectEntries;
    for (int i = 0; i < header->ObjectCount; i++) {
        ObjectEntry* objectEntry = *(header->Objects + i);

        objectEntries.push_back(objectEntry);
    }

    return SetData::SetData(objectEntries);
 }

 SetData::~SetData() {

 }

 void SetData::Print() const {
    std::cout << "----------" << std::endl;
    for (ObjectEntry* oe : this->m_ObjectEntries) {
        std::cout << "Name: " << oe->ObjectName << std::endl;
        std::cout << "Type: " << oe->ObjectType << std::endl;
        std::cout << "Unknown Vector 1: " << oe->UnknownVector1.X << " " << oe->UnknownVector1.Y << " " << oe->UnknownVector1.Z << " " << oe->UnknownVector1.W << std::endl;
        std::cout << "Unknown Vector 2: " << oe->UnknownVector2.X << " " << oe->UnknownVector2.Y << " " << oe->UnknownVector2.Z << " " << oe->UnknownVector2.W << std::endl;
        std::cout << "Position: " << oe->Position.X << " " << oe->Position.Y << " " << oe->Position.Z << std::endl;
        std::cout << "Rotation: " << oe->Rotation.X << " " << oe->Rotation.Y << " " << oe->Rotation.Z << std::endl;
        std::cout << "Child Position: " << oe->ChildPositionOffset.X << " " << oe->ChildPositionOffset.Y << " " << oe->ChildPositionOffset.Z << std::endl;
        std::cout << "Child Rotation: " << oe->ChildRotationOffset.X << " " << oe->ChildRotationOffset.Y << " " << oe->ChildRotationOffset.Z << std::endl;

        std::cout << "Tags: ";
        for (int i = 0; i < oe->TagCount; i++) {
            Tag* tag = *(oe->TagOffsetTable + i);
            PrintTag(tag);
        }
        std::cout << std::endl;

        std::cout << "----------" << std::endl;
    }

    std::cout << "Count: " << this->m_ObjectEntries.size() << std::endl;
 }

 void SetData::PrintTag(Tag* tag) {
     if (std::string(tag->TagType) == TAG_TYPE_RANGE_SPAWNING) {
        RangeSpawning* rs = (RangeSpawning*)tag;
        std::cout << TAG_TYPE_RANGE_SPAWNING << " (" << rs->Data->RangeIn << ", " << rs->Data->RangeOut << ") ";
    }
 }
}
