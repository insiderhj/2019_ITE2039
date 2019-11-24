#include <string>
#include <set>
#include <vector>
#include <iostream>

using std::vector;
using std::set;
using std::string;

typedef vector<int> cost_t;

vector<string> split(string str);
vector<int> str_to_int(vector<string> strs);
vector<cost_t> get_costs(int cities, int city_pairs);
int mst(int cities, vector<cost_t> costs);
bool compare_cost(cost_t& a, cost_t& b);
int find_set(vector< set<int> > sets, int n);
void set_union(vector< set<int> >& sets, int a, int b);

int main() {
    string input;
    vector<string> strings;
    vector<int> numbers;
    vector<cost_t> costs;
    int cities, city_pairs, result;

    while (true) {
        std::getline(std::cin, input);

        // split by ' ' or '\t'
        strings = split(input);

        if (strings.size() != 2) {
            break;
        }

        try {
            numbers = str_to_int(strings);

            cities = numbers[0];
            city_pairs = numbers[1];

            costs = get_costs(cities, city_pairs);
            result = mst(cities, costs);

            if (result == -1) std::cout << "cannot create mst" << std::endl;
            else std::cout << result << std::endl;

        } catch (std::invalid_argument e) {
            break;
        } catch (int e) {
            break;
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

vector<cost_t> get_costs(int cities, int city_pairs) {
    string input;
    vector<string> strings;
    vector<int> numbers;
    vector<cost_t> result;
    for (int i = 0; i < city_pairs; i++) {
        std::getline(std::cin, input);

        // split by ' ' or '\t'
        strings = split(input);

        if (strings.size() != 3) {
            throw 0;
        }

        numbers = str_to_int(strings);
        if (numbers[0] > cities || numbers[1] > cities) {
            throw 1;
        }
        result.push_back(numbers);
    }
    return result;
}

int mst(int cities, vector<cost_t> costs) {
    vector< set<int> > sets;
    int result = 0;

    for (int i = 1; i <= cities; i++) {
        set<int> tmp_set;
        tmp_set.insert(i);
        sets.push_back(tmp_set);
    }

    std::sort(costs.begin(), costs.end(), compare_cost);
    for (int i = 0; i < costs.size(); i++) {
        int set1, set2;
        set1 = find_set(sets, costs[i][0]);
        set2 = find_set(sets, costs[i][1]);
        if (set1 != set2) {
            result += costs[i][2];
            set_union(sets, set1, set2);
        }
    }
    if (sets.size() != 1) return -1;
    return result;
}

bool compare_cost(cost_t& a, cost_t& b) {
    return a[2] < b[2];
}

int find_set(vector< set<int> > sets, int n) {
    for (int i = 0; i < sets.size(); i++) {
        if (sets[i].find(n) != sets[i].end()) return i;
    }
    return -1;
}

void set_union(vector< set<int> >& sets, int a, int b)
{
    sets[a].insert(sets[b].begin(), sets[b].end());
    sets.erase(sets.begin() + b);
}
