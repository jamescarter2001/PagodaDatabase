#include "pgpch.h"

#include "database/common/pg_bina.h"
#include "database/common/pg_bina_reader.h"
#include "database/common/pg_bina_writer.h"
#include "database/common/pg_bina_template_converter.h"

#include "database/set/pg_set.h"
#include "database/model/pg_model.h"

int main() {
    Pagoda::Database::BINATemplateConverter conv;
    conv.ConvertTemplateAndSave("res/basic.bt", "../output/test.orc", PTR_SIZE_64);

    std::ifstream orcFile("C:/sonic2010/tools/stg901_obj/stg901_grass.orc", std::ios::binary | std::ios::in);
    Pagoda::Database::BINAV1Header orcHeader;

    READ_STRUCT(orcFile, orcHeader);
    orcFile.seekg(0, std::ios::beg);

    if (orcHeader.signature == binaSig) {
        char* data = new char[orcHeader.fileSize];
        orcFile.read(data, orcHeader.fileSize);

        Pagoda::Database::Node::PrintOffsets(data + sizeof(orcHeader) + orcHeader.offsetTableOffset, orcHeader.offsetTableLength, sizeof(orcHeader));

        delete[] data;
    }

    float cubeArray[] = {
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
       -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 3, 4,
        5, 6, 3,
        7, 3, 6,
        2, 4, 7,
        0, 7, 6,
        0, 5, 1,
        1, 5, 3,
        5, 0, 6,
        7, 4, 3,
        2, 1, 4,
        0, 2, 7
    };

    char en1[4] = "POS";
    char en2[4] = "COL";

    Pagoda::Database::VertexBufferElement e1 = {en1, 1};
    Pagoda::Database::VertexBufferElement e2 = {en2, 2};

    std::vector<Pagoda::Database::VertexBufferElement*> elements;
    elements.push_back(&e1);
    elements.push_back(&e2);

    Pagoda::Database::ModelHeader cubeHeader = {0};

    cubeHeader.vertexBuffer = cubeArray;
    cubeHeader.indexBuffer = indices;
    cubeHeader.elementArray = &elements[0];
    cubeHeader.vertexBufferSize = (unsigned int)sizeof(cubeArray);
    cubeHeader.indexBufferSize = (unsigned int)sizeof(indices);
    cubeHeader.elementCount = (unsigned int)elements.size();

    Pagoda::Database::BinaWriter bw;

    bw.AddStruct(&cubeHeader, sizeof(cubeHeader));

    bw.AddStruct(cubeArray, sizeof(cubeArray));
    bw.AddStruct(indices, sizeof(indices));
    bw.AddStructVector<Pagoda::Database::VertexBufferElement>(elements);

    bw.AddString(en1);
    bw.AddString(en2);
    bw.Write("../output/dummy_cube.orc");

    Pagoda::Database::BinaReader binaReader;

    std::vector<data_t*> binaFile = binaReader.Read("C:/w1r03_gedit/w1r03_autotest.gedit");
    Pagoda::Database::SetData setData = Pagoda::Database::SetData::SetDataFromNodeData(binaFile[0]);
    setData.Print();

    return 0;
}