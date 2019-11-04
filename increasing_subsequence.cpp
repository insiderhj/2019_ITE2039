#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <utility>

using std::vector;
using std::string;

vector< std::pair<int, int> > get_lis(string str);
void print_lis(string str, vector< std::pair<int, int> > v);
int get_max_idx(vector< std::pair<int, int> > v);

int main() {
    string input, output;
    vector< std::pair<int, int> > v;

    while (1) {
        std::getline(std::cin, input);
        input = input.substr(0, input.find_first_of("\t "));

        if (input.size() == 0) break;

        v = get_lis(input);
        print_lis(input, v);
    }

    return 0;
}

// pair <before_index, string_length>
vector< std::pair<int, int> > get_lis(string str) {
    vector< std::pair<int, int> > v;
    for (int i = 0; i < str.size(); i++) {
        v.push_back(std::make_pair(i, 1));
    }

    for (int i = 0; i < str.size(); i++) {
        for (int j = 0; j < i; j++) {
            if (str[i] > str[j] && v[i].second < v[j].second + 1) v[i] = std::make_pair(j, v[j].second + 1);
        }
    }

    return v;
}

void print_lis(string str, vector< std::pair<int, int> > v) {
    string res;
    int i = get_max_idx(v);
    
    while (v[i].second != 1) {
        res += str[i];
        i = v[i].first;
    }
    res += str[i];

    std::reverse(res.begin(), res.end());
    std::cout << res << std::endl;
}

int get_max_idx(vector< std::pair<int, int> > v) {
    int res, tmp_len = 0;
    for (int i = 0; i < v.size(); i++) {
        if (v[i].second > tmp_len) {
            res = i;
            tmp_len = v[i].second;
        }
    }

    return res;
}
