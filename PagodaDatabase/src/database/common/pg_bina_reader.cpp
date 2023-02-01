#include "btpch.h"

#include "pg_bina_reader.h"

namespace Pagoda::Database {
    BinaReader::BinaReader() {
    }

    BinaReader::~BinaReader() {
    }

    Bina BinaReader::ReadLegacy(std::string path) {
        std::ifstream file(path, std::ios::binary | std::ios::in);

        Header binaHeader;
        READ_STRUCT(file, binaHeader);

        Bina binaFile;

        for (int i = 0; i < binaHeader.nodeCount; i++) {
            NodeHeader nodeHeader;
            READ_STRUCT(file, nodeHeader);

            NodeBody* nodeBody = new NodeBody(nodeHeader);

            READ_BYTES(file, nodeBody->additionalData, nodeHeader.additionalDataLength);
            READ_BYTES(file, nodeBody->dataBlock, nodeHeader.stringTableOffset);
            READ_BYTES(file, nodeBody->stringTable, nodeHeader.stringTableLength);
            READ_BYTES(file, nodeBody->offsetTable, nodeHeader.offsetTableLength);

            Node* node = new Node(nodeHeader, nodeBody);
            binaFile.AddNode(node);
        }

        return binaFile;
    }

    std::vector<data_t*> BinaReader::Read(std::string path) {
        std::ifstream file(path, std::ios::binary | std::ios::in);

        Header binaHeader;
        READ_STRUCT(file, binaHeader);

        file.seekg(0, std::ios::beg);

        bina_t* bina = new bina_t[binaHeader.fileSize];
        file.read(bina, binaHeader.fileSize);

        std::vector<node_t*> nodes;
        std::vector<data_t*> dataBlocks;

        // Create pointers to each node.
        node_t* current = bina + sizeof(binaHeader);
        for (int i = 0; i < binaHeader.nodeCount; i++) {
            nodes.push_back(current);

            NodeHeader* nh = (NodeHeader*) current;
            unsigned long long dataBlockStart = (unsigned long long) current + sizeof(*nh) + nh->additionalDataLength;

            std::vector<unsigned long long> offsets = Node::SeekOffsets(current);

            // Fix pointers in memory.
            for (unsigned long long o : offsets) {
                unsigned long long* ptr = (unsigned long long*)o;
                unsigned long long unaltered = *ptr;
                *ptr = *ptr + dataBlockStart;

                char* test = (char*) *ptr;
            }

            dataBlocks.push_back(current + sizeof(*nh) + nh->additionalDataLength);

            current += nh->length;
        }

        return dataBlocks;
    }

    void BinaReader::ResolveOffsets(node_t* n) {

    }

}