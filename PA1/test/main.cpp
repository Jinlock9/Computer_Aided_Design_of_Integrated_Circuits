#include "main.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

int main(int argc, char* argv[]) {
    std::string bench = argv[1];
    std::string value = argv[2];
    std::ifstream netlist (bench);

    std::string raw;

    while (getline(netlist, raw)) {
        if (!raw.empty() && !raw.find_first_not_of("#")) {
            std::string line = remove_space(raw);
            if (line.find("INPUT") == 0 || line.find("input") == 0) {
                getGate(line);
            }
            else if (line.find("OUTPUT") == 0 || line.find("output") == 0) {
                getGate(line);
            }
            else {
                std::cout << line << std::endl;
            }
        }
    }

    netlist.close();

    return 0;
}

auto remove_space(std::string str) -> std::string {
    std::string filtered = "";
    for (std::size_t i = 0; i < str.length(); i++) {
        if(str[i] != ' ') {
            filtered.push_back(str[i]);
        }
    }

    return filtered;
}

void getGate(std::string str) {
    std::size_t r = str.find("(");
    std::size_t l = str.rfind(")");
    std::string raw = str.substr(r + 1, l - r -1);
    std::cout << raw << std::endl;
}