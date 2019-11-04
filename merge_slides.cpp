#include <iostream>
#include <vector>
#include <string>
#include <utility>

// element_t: {TOTAL_COST, TOTAL_PAGES, SPLIT_INDEX}
#define TOTAL_COST 0
#define TOTAL_PAGES 1
#define SPLIT_INDEX 2

using std::vector;
using std::string;

typedef vector<int> element_t;

// function redefinitions
// base
vector<string> split(string str);
vector<int> str_to_int(vector<string> strs);
vector< vector<element_t> > init_table(vector<int> nums);

// construct table
void fill_table(vector< vector<element_t> >& v);
element_t get_min_cost(vector< vector<element_t> > v, int row, int col);

// result
string get_result(vector< vector<element_t> > table, int row, int col);

int main() {
    string input, result;
    vector<string> strings;
    vector<int> numbers;
    vector< vector<element_t> > table;
    
    while (true) {
        std::getline(std::cin, input);

        // split by ' ' or '\t'
        strings = split(input);

        // if empty input, terminate program
        if (strings.size() == 0) break;

        try {
            numbers = str_to_int(strings);
            table = init_table(numbers);

            // check whether to print calculation order or not
            if (table.size() > 1) {
                fill_table(table);
                result = get_result(table, 0, table.size() - 1);
            } else {
                result = "";
            }

            std::cout << table[0][table.size() - 1][TOTAL_COST] << ' ' << result << std::endl;
        } catch (std::invalid_argument e) {
            std::cout << "invalid input: not a number" << std::endl;
        } catch (int e) {
            std::cout << "invalid value: maybe 0 or less" << std::endl;
        }
    }
    
    return 0;
}

/**
 * split string by ' ' and '\t'.
 * 
 * @param str original string
 * @return vector of strings splitted by ' ' and '\t'
 */
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

/**
 * change a vector of strings to a vector of ints.
 * 
 * @param strs vector of strings
 * @return vector of ints converted from string
 * @throw std::invalid_argument thrown by std::stoi
 * @throw int 1 thrown when number is equal to or less then zero
 */
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

/**
 * initialize element table with total_cost 0
 * 
 * @param nums vector of input numbers
 * @return 2d vector with initial settings
 */
vector< vector<element_t> > init_table(vector<int> nums) {
    vector< vector<element_t> > v;
    vector<element_t> tmp;
    element_t e;

    // initialize vector
    e.push_back(0);
    e.push_back(0);
    e.push_back(0);
    tmp.push_back(e);

    for (int i = 0; i < nums.size(); i++) {
        // set total_pages of tmp then add to table
        tmp[0][TOTAL_PAGES] = nums[i];
        v.push_back(tmp);
    }

    return v;
}

/**
 * fill table with elements.
 * 
 * @param v main table to fill in
 */
void fill_table(vector< vector<element_t> >& v) {
    // fill in the table from 1 to v.size(), from bottom to top
    // ex) if size() == 4, fill
    //     [3, 1] -> [2, 1] -> [1, 1] -> [0, 1] ->
    //     [2, 2] -> [1, 2] -> [0, 2] ->
    //     [1, 3] -> [0, 3] ->
    //     [0, 4]
    for (int i = 0; i < v.size() - 1; i++) {
        for (int j = i; j >= 0; j--) {
            v[j].push_back(get_min_cost(v, j, i + 1 - j));
        }
    }
}

/**
 * get minimum total_cost that can fit in v[row][col].
 *
 * @param v container vector of elements
 * @param row row number of element
 * @param col column number of element
 * @return vector containing minimum total_cost with total_pages and split_index
 */
element_t get_min_cost(vector< vector<element_t> > v, int row, int col) {
    element_t res;
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

/**
 * get result string which shows calculation order.
 * 
 * @param table main table with elements
 * @param row row number of result
 * @param col column number of result
 * @return numbers enclosed in parentheses by calculation order
 */
string get_result(vector< vector<element_t> > table, int row, int col) {
    string res;
    if (col == 0) {
        return std::to_string(table[row][col][TOTAL_PAGES]);
    }

    res = "(" + get_result(table, row, table[row][col][SPLIT_INDEX]) + "," +
        get_result(table,
                   row + table[row][col][SPLIT_INDEX] + 1,
                   col - table[row][col][SPLIT_INDEX] - 1) +
        ")";
    return res;
}
