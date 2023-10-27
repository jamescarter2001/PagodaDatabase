#pragma once

struct PACTable {
    unsigned int count;
    void* pacTableEntries;
};

struct ResPacDepend {
    char* pPacNames;
    unsigned int count;
};