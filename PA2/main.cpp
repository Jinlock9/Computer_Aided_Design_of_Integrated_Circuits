#include "main.hpp"

int num_vars;
cubeList cube_list;

int main(int argc, char* argv[]) {
    init(argv[1]);
    execute();

    return 0;
}

void init(std::string str) {
    std::ifstream file (str);
    std::string line;

    if (getline(file, line) && !line.empty()) {
        num_vars = std::stoi(line);
    }
    while (getline(file, line) && !line.empty()) {
        std::vector<unsigned int> cube;
        for (char ch : line) {
            unsigned int edge;
            if (ch == '1') edge = 1;
            else if (ch == '0') edge = 2;
            else if (ch == '-') edge = 3;
            else edge = 0;
            cube.push_back(edge);
        }
        cube_list.cubes.push_back(cube);
        cube_list.num_cubes++;
    }
    file.close();
}

cubeList complement(cubeList list) {
    // Termination Conditions
    if (list.num_cubes == 0) { 
        // [1] Empty cube list
        list.num_cubes++;
        std::vector<unsigned int> temp;
        for (int i = 0; i < num_vars; i++) {
            temp.push_back(3);
        }
        list.cubes.push_back(temp);
        return list;
    }
    // [3] Cube list contains All-Don't-Cares Cube
    for (auto cube : list.cubes) {
        if (is_all_dont_cares(cube)) {
            cubeList new_list;
            new_list.num_cubes = 0;
            return new_list;
        }
    }
    if (list.num_cubes == 1) {
        return demorgan(list.cubes[0]);
        // [2] Cube list contains just one cube
    }

    int x = selection(list);

    cubeList p = complement(cofactor(list, x, 1));
    cubeList n = complement(cofactor(list, x, 0));

    p = op_and(p, x, 1);
    n = op_and(n, x, 0);

    return op_or(p, n);
}

int is_all_dont_cares(std::vector<unsigned int> cube) {
    int value = 1;
    for (auto edge : cube) {
        if (edge != 3) {
            value = 0;
            break;
        }
    }
    return value;
}

cubeList demorgan(std::vector<unsigned int> cube) {
    cubeList new_list;
    new_list.num_cubes = 0;
    for (int i = 0; i < num_vars; i++) {
        if (cube[i] != 3) {
            std::vector<unsigned int> temp;
            for (int j = 0; j < num_vars; j++) {
                if (j == i) {
                    unsigned int comp;
                    if (cube[i] == 1) comp = 2;
                    else comp = 1;
                    temp.push_back(comp);
                }
                else {
                    temp.push_back(3);
                }
            }
            new_list.num_cubes++;
            new_list.cubes.push_back(temp);
        }
    }
    return new_list;
}

int selection(cubeList list) {
    int index;
    int max_num = -1;
    int min_balance = num_vars + 1;
    int no_binate = 1;
    std::unordered_map<int, int> map1;
    std::unordered_map<int, int> map2;
    for (auto cube : list.cubes) {
        for (int i = 0; i < num_vars; i++) {
            if (cube[i] != 3) {
                map1[i]++;
                if (cube[i] == 1) {
                    map2[i]++;
                }
                else {
                    map2[i]--;
                }
            }
        }
    }
    for (int j = 0; j < num_vars; j++) {
        if (std::abs(map1[j]) != std::abs(map2[j])) {
            if (map1[j] > max_num) {
                index = j;
                max_num = map1[j];
                min_balance = map2[j];
            }
            else if (map1[j] == max_num) {
                if (map2[j] < min_balance) {
                    index = j;
                    min_balance = map2[j];
                }
            }
            no_binate = 0;
        }
    }
    if (no_binate) {
        for (int k = 0; k < num_vars; k++) {
            if (map1[k] > max_num) {
                index = k;
                max_num = map1[k];
            }
        }
    }
    return index;
}

cubeList cofactor(cubeList list, int index, int sign) {
    cubeList new_list;
    new_list.num_cubes = 0;
    if (sign > 0) { // Positive Cofactor
        for (auto cube : list.cubes) {
            if (cube[index] != 2) {
                std::vector<unsigned int> temp;
                for (int i = 0; i < num_vars; i ++) {
                    if (i == index) {
                        temp.push_back(0b11);
                    }
                    else {
                        temp.push_back(cube[i]);
                    }
                }
                new_list.num_cubes++;
                new_list.cubes.push_back(temp);
            }
        }
    }
    else {
        for (auto cube : list.cubes) {
            if (cube[index] != 1) {
                std::vector<unsigned int> temp;
                for (int i = 0; i < num_vars; i ++) {
                    if (i == index) {
                        temp.push_back(0b11);
                    }
                    else {
                        temp.push_back(cube[i]);
                    }
                }
                new_list.num_cubes++;
                new_list.cubes.push_back(temp);
            }
        }
    }
    return new_list;
}

cubeList op_and(cubeList list, int index, int sign) {
    cubeList new_list;
    new_list.num_cubes = 0;
    if (sign > 0) {
        for (auto cube : list.cubes) {
            std::vector<unsigned int> temp;
            for (int i = 0; i < num_vars; i ++) {
                if (i == index) {
                    temp.push_back(1);
                }
                else {
                    temp.push_back(cube[i]);
                }
            }
            new_list.num_cubes++;
            new_list.cubes.push_back(temp);
        }
    }
    else {
        for (auto cube : list.cubes) {
            std::vector<unsigned int> temp;
            for (int i = 0; i < num_vars; i ++) {
                if (i == index) {
                    temp.push_back(2);
                }
                else {
                    temp.push_back(cube[i]);
                }
            }
            new_list.num_cubes++;
            new_list.cubes.push_back(temp);
        }
    }
    return new_list;
}

cubeList op_or(cubeList list1, cubeList list2) {
    cubeList new_list;
    new_list.num_cubes = 0;
    for (auto cube : list1.cubes) {
        new_list.cubes.push_back(cube);
        new_list.num_cubes++;
    }
    for (auto cube : list2.cubes) {
        new_list.cubes.push_back(cube);
        new_list.num_cubes++;
    }
    return new_list;
}

void execute() {
    cube_list = complement(cube_list);

    std::cout << num_vars << std::endl;
    if (cube_list.num_cubes > 0) {
        for (auto cube : cube_list.cubes) {
            for (auto edge : cube) {
                switch (edge) {
                    case 1:
                        std::cout << "1";
                        break;
                    case 2:
                        std::cout << "0";
                        break;
                    case 3:
                        std::cout << "-";
                        break;
                    default:
                        std::cout << "E";
                }
            }
            std::cout << std::endl;
        }
    }
}