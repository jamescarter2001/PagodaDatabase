#include "btpch.h"

#include "database/common/pg_bina_raw.h"
#include "database/common/pg_bina_reader.h"

#include "database/set/pg_set.h"

int main() {
    Pagoda::Database::BinaReader binaReader;

    std::vector<data_t*> binaFile = binaReader.Read("E:/w1r03_gedit/w1r03_plan_action_00.gedit");
    Pagoda::Database::SetData setData = Pagoda::Database::SetData::SetDataFromNodeData(binaFile[0]);
    setData.Print();

    return 0;
}