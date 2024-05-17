#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    std::string line;
    std::ifstream file (argv[1]);

    while (getline(file, line)) {
        if (!line.empty() && !line.find_first_not_of("#")) {
            std::cout << line << std::endl;
        }
    }

    file.close();

    return 0;
}