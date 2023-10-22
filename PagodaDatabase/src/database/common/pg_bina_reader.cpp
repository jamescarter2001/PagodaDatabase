#include "pgpch.h"

#include "pg_bina_reader.h"

namespace Pagoda::Database {
    BinaReader::BinaReader() {
    }

    BinaReader::~BinaReader() {
    }

    std::vector<data_t*> BinaReader::Read(std::string path) {
        std::ifstream file(path, std::ios::binary | std::ios::in);

        BINAV1Header binav1Header;
        READ_STRUCT(file, binav1Header);
        file.seekg(0, std::ios::beg);

        BINAHeader binaHeader;
        READ_STRUCT(file, binaHeader);
        file.seekg(0, std::ios::beg);

        bina_t* bina;
        std::vector<data_t*> dataBlocks;
        if (binav1Header.signature == binaSig) {
            std::cout << "Reading BINA: V1" << std::endl;
            bina = new bina_t[binav1Header.fileSize];
            file.read(bina, binav1Header.fileSize);

            dataBlocks = ReadV1(bina);
        }

        if (binaHeader.header == binaSig) {
            std::cout << "Reading BINA: V2" << std::endl;
            bina = new bina_t[binaHeader.fileSize];
            file.read(bina, binaHeader.fileSize);

            dataBlocks = ReadV2(bina);
        }
        
        return dataBlocks;
    }

    std::vector<data_t*> BinaReader::ReadV1(bina_t* bina) {
        BINAV1Header binaHeader = *(BINAV1Header*)bina;
        data_t* dataBlock = bina + sizeof(binaHeader);

        std::vector<unsigned long long> offsets = Node::SeekOffsets((unsigned long long)bina + sizeof(binaHeader), bina + sizeof(binaHeader) + binaHeader.offsetTableOffset, binaHeader.offsetTableLength);

        // Fix pointers in memory.
        for (unsigned long long o : offsets) {
            unsigned long long* ptr = (unsigned long long*)o;
            *ptr = *ptr + (unsigned long long)dataBlock;
        }

        return std::vector<data_t*>({dataBlock});
    }

    // Direct memory loading.
    std::vector<data_t*> BinaReader::ReadV2(bina_t* bina) {

        BINAHeader binaHeader = *(BINAHeader*)bina;

        std::vector<node_t*> nodes;
        std::vector<node_t*> dataBlocks;

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

            dataBlocks.push_back(current + sizeof(NodeHeader));

            current += nh->length;
        }
        return dataBlocks;
    }
}