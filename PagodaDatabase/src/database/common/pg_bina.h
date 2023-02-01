#pragma once
#include "pgpch.h"

#include "pg_bina_raw.h"

typedef char bina_t;
typedef char node_t;
typedef char data_t;

#define BINA_OFFSET_END 0b00000000
#define BINA_OFFSET_6 0b01000000
#define BINA_OFFSET_14 0b10000000
#define BINA_OFFSET_30 0b11000000

namespace Pagoda::Database {
    class Node {
    public:
        Node(NodeHeader& nh, NodeBody* nb) : nodeHeader(nh), nodeBody(nb) {}

        NodeHeader nodeHeader;
        NodeBody* nodeBody;

        std::vector<unsigned long> SeekOffsets(unsigned long currentOffset = 0);
        static std::vector<unsigned long long> SeekOffsets(node_t* node);
    };

    class Bina {
    public:
        Bina() {
            this->m_Header = Header();
        }

        Bina(Header& header) {
            this->m_Header = header;
        }

        virtual ~Bina() {
            for (Node* n : this->m_Nodes) {
                delete n;
            }
        }

        void AddNode(Node* node) {
            m_Nodes.push_back(node);
        }

        inline Header GetHeader() {
            return this->m_Header;
        }

        inline std::vector<Node*> GetNodes() {
            return this->m_Nodes;
        }

    private:
        Header m_Header;
        std::vector<Node*> m_Nodes;
    };
}