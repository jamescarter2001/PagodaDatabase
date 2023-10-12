#include "pgpch.h"

#include "database/common/pg_bina_reader.h"

#include "database/set/pg_set.h"
#include "database/model/pg_model.h"
#include "database/common/pg_bina_writer.h"

int main() {
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

    Pagoda::Database::ModelHeader cubeHeader = {cubeArray, indices, &elements[0], (unsigned int)sizeof(cubeArray), (unsigned int)sizeof(indices), (unsigned int)elements.size()};

    std::cout << std::hex << &elements[0] << std::endl;

    Pagoda::Database::BinaWriter bw;

    bw.AddStruct(&cubeHeader, sizeof(cubeHeader));
    bw.AddStruct(cubeArray, sizeof(cubeArray));
    bw.AddStruct(indices, sizeof(indices));
    bw.AddStructVector<Pagoda::Database::VertexBufferElement>(elements);

    bw.AddStruct(en1, sizeof(en1), true);
    bw.AddStruct(en2, sizeof(en2), true);
    bw.Write("../output/dummy_cube.orc");

    std::cout << elements[0]->name << std::endl;

    Pagoda::Database::BinaReader binaReader;

    for (auto e : elements) {
        std::cout << e->name << std::endl;
    }

    //std::vector<data_t*> binaFile = binaReader.Read("E:/w9d02_gedit/w9d02_autotest.gedit");
    std::vector<data_t*> binaFile2 = binaReader.Read("../output/dummy_cube.orc");
    Pagoda::Database::ModelData modelData = Pagoda::Database::ModelData::ModelDataFromNodeData(binaFile2[0]);
    modelData.Print();
    //Pagoda::Database::SetData setData = Pagoda::Database::SetData::SetDataFromNodeData(binaFile[0]);
    //setData.Print();

    return 0;
}