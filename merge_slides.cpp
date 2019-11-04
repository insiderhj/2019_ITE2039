#include <iostream>
#include <vector>
#include <string>
#include <utility>

#define TOTAL_COST 0
#define TOTAL_PAGES 1
#define SPLIT_INDEX 2

typedef std::vector<int> element;

std::vector<std::string> split(std::string str);
std::vector<int> str_to_int(std::vector<std::string> strs);
std::vector< std::vector<element> > init_table(std::vector<int> nums);

void fill_table(std::vector< std::vector<element> >& v);
element get_min_element(std::vector< std::vector<element> > v, int row, int col);

std::string get_result(std::vector< std::vector<element> > table, int row, int col);

int main() {
    std::string input, result;
    std::vector<std::string> strings;
    std::vector<int> numbers;
    std::vector< std::vector<element> > table;
    
    while (true) {
        std::getline(std::cin, input);
        strings = split(input);

        if (strings.size() == 0) break;

        try {
            numbers = str_to_int(strings);
            table = init_table(numbers);

            if (table.size() > 1) fill_table(table);
            
            result = get_result(table, 0, table.size() - 1);
            std::cout << table[0][table.size() - 1][TOTAL_COST] << ' ' << result << std::endl;
        } catch (std::invalid_argument e) {
            std::cout << "invalid input: not a number" << std::endl;
        } catch (int e) {
            std::cout << "invalid value: maybe 0 or less" << std::endl;
        }
    }
    
    return 0;
}

std::vector<std::string> split(std::string str) {
    std::vector<std::string> v;
    size_t start;

    if ((start = str.find_first_not_of(" \t")) < str.size()) str = str.substr(start);
    else return v;

    while (str.size() > 0) {
        v.push_back(str.substr(0, str.find_first_of(" \t")));

        if ((start = str.find_first_of(" \t")) < str.size())
            str = str.substr(start);
        else break;

        if ((start = str.find_first_not_of(" \t")) < str.size())
            str = str.substr(start);
        else break;
    }

    return v;
}

std::vector<int> str_to_int(std::vector<std::string> strs) {
    std::vector<int> v;
    int input;
    
    for (size_t i = 0; i < strs.size(); i++) {
        input = std::stoi(strs[i]);
        if (input > 0) v.push_back(input);
        else throw 1;
    }

    return v;
}

std::vector< std::vector<element> > init_table(std::vector<int> nums) {
    std::vector< std::vector<element> > v;
    std::vector<element> tmp;
    element e;

    e.push_back(0);
    e.push_back(0);
    e.push_back(0);

    tmp.push_back(e);
    for (int i = 0; i < nums.size(); i++) {
        tmp[0][TOTAL_PAGES] = nums[i];
        v.push_back(tmp);
    }

    return v;
}

void fill_table(std::vector< std::vector<element> >& v) {
    for (int i = 0; i < v.size() - 1; i++) {
        element tmp;
        tmp.push_back(v[i][0][TOTAL_PAGES] + v[i + 1][0][TOTAL_PAGES]);
        tmp.push_back(v[i][0][TOTAL_PAGES] + v[i + 1][0][TOTAL_PAGES]);
        tmp.push_back(0);
        v[i].push_back(tmp);
    }

    for (int i = 0; i < v.size() - 2; i++) {
        for (int j = i; j >= 0; j--) {
            v[j].push_back(get_min_element(v, j, i + 2 - j));
        }
    }
}

element get_min_element(std::vector< std::vector<element> > v, int row, int col) {
    element res;
    res.push_back(INT_MAX);
    res.push_back(v[row][0][TOTAL_PAGES] + v[row + 1][col - 1][TOTAL_PAGES]);
    res.push_back(0);
    for (int i = 0; i < col; i++) {
        int tmp = v[row][i][TOTAL_COST] + v[row][i][TOTAL_PAGES] +
                  v[row + i + 1][col - i - 1][TOTAL_COST] + v[row + i + 1][col - i - 1][TOTAL_PAGES];
        
        if (res[TOTAL_COST] > tmp) {
            res[TOTAL_COST] = tmp;
            res[SPLIT_INDEX] = i;
        }
    }

    return res;
}

std::string get_result(std::vector< std::vector<element> > table, int row, int col) {
    std::string res;
    if (col == 0) {
        return std::to_string(table[row][col][TOTAL_PAGES]);
    }

    res = "(" + get_result(table, row, table[row][col][SPLIT_INDEX]) + "," +
        get_result(table, row + table[row][col][SPLIT_INDEX] + 1, col - table[row][col][SPLIT_INDEX] - 1) + ")";
    return res;
}
