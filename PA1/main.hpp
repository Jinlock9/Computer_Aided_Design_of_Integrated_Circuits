#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

enum Type {
    AND, OR, NAND, NOR, XOR, XNOR, INV, BUF, ERROR
};

struct Gate {
    Type type;
    std::string output;
    std::vector<std::string> input;
};

std::string remove_space(std::string str);
std::string get_signal(std::string str);
void get_gate(std::string str);
Type get_type(std::string type);
bool get_output(Type type, std::vector<std::string> input);

void read_bench(std::string str);
void read_values(std::string str);
void run_circuit();
void print();

#endif // MAIN_H