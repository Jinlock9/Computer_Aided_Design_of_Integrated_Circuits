#include "main.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <unordered_map>

std::vector<Gate> gates;
std::unordered_map<std::string, bool> inputs;
std::vector<std::string> outputs;

int main(int argc, char* argv[]) {
    std::string bench = argv[1];
    std::string val = argv[2];

    read_bench(bench);
    read_values(val);

    run_circuit();
    print();

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

void get_gate(std::string str) {
    Gate gate;
    std::size_t eq = str.find("=");
    std::size_t rb = str.find("(");
    std::size_t lb = str.find(")");

    gate.output = str.substr(0, eq);
    gate.type = get_type(str.substr(eq + 1, rb - eq - 1));
    
    std::string in;
    for (std::size_t i = rb + 1; i < lb; i++) {
        if (str[i] == ',') {
            gate.input.push_back(in);
            in = "";
        }
        else {
            in.push_back(str[i]);
        }
    }
    gate.input.push_back(in);
    gates.push_back(gate);
}

Type get_type(std::string type) {
    if (type == "AND" || type == "and") {
        return AND;
    }
    else if (type == "OR" || type == "or") {
        return OR;
    }
    else if (type == "NAND" || type == "nand") {
        return NAND;
    }
    else if (type == "NOR" || type == "nor") {
        return NOR;
    }
    else if (type == "XOR" || type == "xor") {
        return XOR;
    }
    else if (type == "XNOR" || type == "xnor") {
        return XNOR;
    }
    else if (type == "NOT" || type == "not") {
        return INV;
    }
    else if (type == "BUF" || type == "buf") {
        return BUF;
    }
    else {
        return ERROR;
    }
}

bool get_output(Type type, std::vector<std::string> input) {
    bool output = false; // default
    if (type == AND) {
        output = true;
        for (std::string in : input) {
           if (!inputs[in]) {
                output = false;
           }
        }
    }
    else if (type == OR) {
        output = false;
        for (std::string in : input) {
           if (inputs[in]) {
                output = true;
           }
        }
    }
    else if (type == NAND) {
        output = true;
        for (std::string in : input) {
           if (!inputs[in]) {
                output = false;
           }
        }
        output = !output;
    }
    else if (type == NOR) {
        output = false;
        for (std::string in : input) {
           if (inputs[in]) {
                output = true;
           }
        }
        output = !output;
    }
    else if (type == XOR) {
        for (std::string in : input) {
            output ^= inputs[in];
        }
    }
    else if (type == XNOR) {
        for (std::string in : input) {
            output ^= inputs[in];
        }
        output = !output;
    }
    else if (type == INV) {
        output = !inputs[input[0]];
    }
    else if (type == BUF) {
        output = inputs[input[0]];
    }
    return output;
}

void read_bench(std::string str) {
    std::ifstream bench (str);
    std::string raw;
    int checker = 0;
    while (getline(bench, raw)) {
        if (!raw.empty() && !raw.find_first_not_of("#")) {
            std::string line = remove_space(raw);
            if (line.find("OUTPUT") == 0 || line.find("output") == 0) {
                outputs.push_back(get_signal(line));
                checker = 1;
            }
            else if (checker) { // Skipping INPUT
                get_gate(line);
            } 
        }
    }
    bench.close();
}

void read_values(std::string str) {
    std::ifstream val (str);
    std::string raw;
    while (getline(val, raw)) {
        if (!raw.empty()) {
            std::size_t space = raw.find(" ");
            inputs[raw.substr(0, space)] = std::stoi(raw.substr(space + 1));
        }
    }
    val.close();
}

void run_circuit() {
    if (!gates.empty()) {
        for (auto gate : gates) {
            inputs[gate.output] = get_output(gate.type, gate.input);
        }
    }
}

void print() {
    if (!outputs.empty()) {
        for (std::string output : outputs) {
            std::cout << output << " ";
            if (inputs[output]) {
                std::cout << 1 << std::endl;
            }
            else {
                std::cout << 0 << std::endl;
            }
        }
    }
}
