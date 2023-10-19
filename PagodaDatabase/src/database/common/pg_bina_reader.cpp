#include "pgpch.h"

#include "pg_bina_reader.h"

namespace Pagoda::Database {
    BinaReader::BinaReader() {
    }

    BinaReader::~BinaReader() {
    }

    // Direct memory loading.
    std::vector<node_t*> BinaReader::Read(std::string path) {
        std::ifstream file(path, std::ios::binary | std::ios::in);

        BINAHeader binaHeader;
        READ_STRUCT(file, binaHeader);

        file.seekg(0, std::ios::beg);

        bina_t* bina = new bina_t[binaHeader.fileSize];
        file.read(bina, binaHeader.fileSize);

        std::vector<node_t*> nodes;

        // Create pointers to each node.
        node_t* current = bina + sizeof(binaHeader);
        for (int i = 0; i < binaHeader.nodeCount; i++) {
            nodes.push_back(current);

            NodeHeader* nh = (NodeHeader*) current;
            unsigned long long dataBlockStart = (unsigned long long) current + sizeof(*nh);

            std::vector<unsigned long long> offsets = Node::SeekOffsets(current);

            // Fix pointers in memory.
            for (unsigned long long o : offsets) {
                unsigned long long* ptr = (unsigned long long*)o;
                *ptr = *ptr + dataBlockStart;
            }

            current += nh->length;
        }
        return nodes;
    }
}