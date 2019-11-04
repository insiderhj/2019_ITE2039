#include <iostream>
#include <vector>
#include <string>
#include <utility>

#define TOTAL_COST 0
#define TOTAL_PAGES 1
#define SPLIT_INDEX 2

using std::vector;
using std::string;

typedef vector<int> element;

vector<string> split(string str);
vector<int> str_to_int(vector<string> strs);
vector< vector<element> > init_table(vector<int> nums);

void fill_table(vector< vector<element> >& v);
element get_min_element(vector< vector<element> > v, int row, int col);

string get_result(vector< vector<element> > table, int row, int col);

int main() {
    string input, result;
    vector<string> strings;
    vector<int> numbers;
    vector< vector<element> > table;
    
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

vector<string> split(string str) {
    vector<string> v;
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

vector<int> str_to_int(vector<string> strs) {
    vector<int> v;
    int input;
    
    for (size_t i = 0; i < strs.size(); i++) {
        input = std::stoi(strs[i]);
        if (input > 0) v.push_back(input);
        else throw 1;
    }

    return v;
}

vector< vector<element> > init_table(vector<int> nums) {
    vector< vector<element> > v;
    vector<element> tmp;
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

void fill_table(vector< vector<element> >& v) {
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

element get_min_element(vector< vector<element> > v, int row, int col) {
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

string get_result(vector< vector<element> > table, int row, int col) {
    string res;
    if (col == 0) {
        return std::to_string(table[row][col][TOTAL_PAGES]);
    }

    res = "(" + get_result(table, row, table[row][col][SPLIT_INDEX]) + "," +
        get_result(table, row + table[row][col][SPLIT_INDEX] + 1, col - table[row][col][SPLIT_INDEX] - 1) + ")";
    return res;
}
