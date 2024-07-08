#ifndef __MAIN_H__
#define __MAIN_H__

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <unordered_map>

struct cubeList {
    int num_cubes;
    std::vector<std::vector<unsigned int>> cubes;
};

void init(std::string str);
cubeList complement(cubeList list);
int is_all_dont_cares(std::vector<unsigned int> cube);
cubeList demorgan(std::vector<unsigned int> cube);
int selection(cubeList list);
cubeList cofactor(cubeList list, int index, int sign);
cubeList op_and(cubeList list, int index, int sign);
cubeList op_or(cubeList list1, cubeList list2);
void execute();

#endif // __MAIN_H__