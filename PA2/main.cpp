#include "main.hpp"

int num_vars;

int main(int argc, char* argv[]) {
    cubeList cube_list = init(argv[1]);
    cube_list = complement(cube_list);
    print(cube_list);

    return 0;
}

cubeList init(std::string str) {
    cubeList list;
    list.num_cubes = 0;
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
            cube.push_back(edge);
        }
        list.cubes.push_back(cube);
        list.num_cubes++;
    }
    file.close();
    return list;
}

cubeList complement(cubeList list) {
    if (list.num_cubes == 0) { 
        cubeList one;
        one.num_cubes = 1;
        std::vector<unsigned int> temp;
        for (int i = 0; i < num_vars; i++) {
            temp.push_back(3);
        }
        one.cubes.push_back(temp);
        return one;
    }
    for (auto cube : list.cubes) {
        if (is_all_dont_cares(cube)) {
            cubeList zero;
            zero.num_cubes = 0;
            return zero;
        }
    }
    if (list.num_cubes == 1) {
        return demorgan(list.cubes[0]);
    }

    int x = selection(list);

    cubeList p = complement(cofactor(list, x, 1));
    cubeList n = complement(cofactor(list, x, 0));

    p = op_and(p, x, 1);
    n = op_and(n, x, 0);

    return op_or(p, n);
}

int is_all_dont_cares(std::vector<unsigned int> cube) {
    int yes = 1;
    for (auto edge : cube) {
        if (edge != 3) {
            yes = 0;
            break;
        }
    }
    return yes;
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
    int unate = 1;
    std::vector<int> map1(num_vars, 0);
    std::vector<int> map2(num_vars, 0);
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
        if (std::abs(map1[j]) != std::abs(map2[j])) { // binate
            int abs_map2j = std::abs(map2[j]);
            if (map1[j] > max_num) {
                index = j;
                max_num = map1[j];
                min_balance = abs_map2j;
            }
            else if (map1[j] == max_num) {
                if (abs_map2j < min_balance) {
                    index = j;
                    min_balance = abs_map2j;
                }
            }
            unate = 0;
        }
    }
    if (unate) {
        max_num = -1;
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
                        temp.push_back(3);
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
                        temp.push_back(3);
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

void print(cubeList list) {
    std::cout << num_vars << std::endl;
    if (list.num_cubes > 0) {
        for (auto cube : list.cubes) {
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
                        std::cout << "";
                }
            }
            std::cout << std::endl;
        }
    }
    else {
        std::cout << std::endl;
    }
}