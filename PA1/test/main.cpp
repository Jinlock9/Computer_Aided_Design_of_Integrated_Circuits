#include "main.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <unordered_map>

std::vector<Gate> gates;
std::unordered_map<std::string, std::size_t> inputs;

int main(int argc, char* argv[]) {
    std::string bench = argv[1];
    std::string value = argv[2];
    std::ifstream netlist (bench);

    std::vector<std::string> outputs;

    std::string raw;
    while (getline(netlist, raw)) {
        if (!raw.empty() && !raw.find_first_not_of("#")) {
            std::string line = remove_space(raw);
            if (line.find("INPUT") == 0 || line.find("input") == 0) {
                inputs[get_signal(line)] = 0;
            }
            else if (line.find("OUTPUT") == 0 || line.find("output") == 0) {
                outputs.push_back(get_signal(line));
            }
            else {
                getGate(line);
            } 
        }
    }

    netlist.close();

    for (auto input : inputs) {
        std::cout << input.first << " " << input.second << std::endl;
    }

    if (!outputs.empty()) {
        for (std::string output : outputs) {
            std::cout << output << std::endl;
        }
    }

    return 0;
}

std::string remove_space(std::string str) {
    std::string filtered = "";
    for (std::size_t i = 0; i < str.length(); i++) {
        if(str[i] != ' ') {
            filtered.push_back(str[i]);
        }
    }
    return filtered;
}

std::string get_signal(std::string str) {
    std::size_t r = str.find("(");
    std::size_t l = str.rfind(")");
    std::string signal = str.substr(r + 1, l - r - 1);
    return signal;
}

void getGate(std::string str) {
    std::size_t eq = str.find("=");
    std::size_t rb = str.find("(");
    std::size_t lb = str.find(")");
    std::string out = str.substr(0, eq);
    std::string logic = str.substr(eq + 1, rb - eq - 1);
    std::cout << out << " | " << logic << " | ";
    std::string in;
    for (std::size_t i = rb + 1; i < lb; i++) {
        if (str[i] == ',') {
            std::cout << in << " | ";
            in = "";
        }
        else {
            in.push_back(str[i]);
        }
    }
    std::cout << in << std::endl;
}