#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using std::string;
using std::vector;

typedef vector<int> cost_t;

vector<string> split(string str);
vector<int> str_to_int(vector<string> strs);
vector< vector<int> > get_costs(int cities, int city_pairs);
int dijkstra(int nodes, vector<cost_t> costs);
int extract_min(vector<int> q, vector<int> s);

int main()
{
    string input;
    vector<string> strings;
    vector<int> numbers;
    vector<cost_t> costs;
    int nodes, edges, result;

    while (true) {
        std::getline(std::cin, input);

        // split by ' ' or '\t'
        strings = split(input);

        if (strings.size() != 2) {
            break;
        }

        try {
            numbers = str_to_int(strings);

            nodes = numbers[0];
            edges = numbers[1];

            costs = get_costs(nodes, edges);
            result = dijkstra(nodes, costs);

            if (result == INT_MAX) std::cout << "cannot find path" << std::endl;
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
        v.push_back(input);
    }

    return v;
}

vector< vector<int> > get_costs(int nodes, int edges) {
    string input;
    vector<int> tmp;
    vector<string> strings;
    vector<int> numbers;
    vector< vector<int> > result;
    for (int i = 0; i < nodes; i++) {
        tmp.push_back(INT_MAX);
    }
    for (int i = 0; i < nodes; i++) {
        result.push_back(tmp);
        result[i][i] = 0;
    }

    for (int i = 0; i < edges; i++) {
        std::getline(std::cin, input);

        // split by ' ' or '\t'
        strings = split(input);

        if (strings.size() != 3) {
            throw 0;
        }

        numbers = str_to_int(strings);
        if (numbers[0] > nodes || numbers[1] > nodes) {
            throw 1;
        }
        if (numbers[0] <= 0 || numbers[1] <= 0 || numbers[2] < 0) {
            throw 2;
        }

        result[numbers[0] - 1][numbers[1] - 1] = numbers[2];
        result[numbers[1] - 1][numbers[0] - 1] = numbers[2];
    }

    return result;
}

int dijkstra(int nodes, vector< vector<int> > costs) {
    vector<int> q;
    vector<int> s;
    int idx;

    q.push_back(0);
    for (int i = 1; i < nodes; i++) {
        q.push_back(INT_MAX);
    }

    while (s.size() < nodes) {
        idx = extract_min(q, s);
        s.push_back(idx);

        for (int i = 0; i < nodes; i++) {
            if (costs[idx][i] == INT_MAX) continue;
            if (find(s.begin(), s.end(), i) != s.end()) continue;

            if (q[i] > q[idx] + costs[idx][i]) q[i] = q[idx] + costs[idx][i];
        }
    }

    return q[nodes - 1];
}

int extract_min(vector<int> q, vector<int> s) {
    int min_val = INT_MAX, res = -1;
    for (int i = 0; i < q.size(); i++) {
        if (find(s.begin(), s.end(), i) == s.end() && q[i] < min_val) {
            min_val = q[i];
            res = i;
        }
    }
    return res;
}
