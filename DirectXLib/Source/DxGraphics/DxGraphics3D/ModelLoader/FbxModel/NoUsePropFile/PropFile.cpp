#include "PropFile.h"
#include <iostream>
#include <fstream>
#pragma warning(disable : 4996)
namespace model {
    void PropFile::outputPreIndex(const char* FileName, std::vector<int>* m_PreIndex) {
        FILE* fp = fopen(FileName, "w");
        std::vector<int> data[4];
        for (int ite = 0; ite < 4; ite++)data[ite] = m_PreIndex[ite];
        fwrite(&data, sizeof(int)*data->size(), 4, fp);
        fclose(fp);
    }
    bool PropFile::inputPreIndex(const char* FileName, std::vector<int>* m_PreIndex) {
        FILE* fp = fopen(FileName, "r");
        if (fp == 0) {
            return false;
        }
        std::vector<int> data[4];
        fread(&data, sizeof(int), m_PreIndex->size() * 4, fp);
        for (int ite = 0; ite < 4; ite++)m_PreIndex[ite] = data[ite];
        fclose(fp);
        return true;
    }
}