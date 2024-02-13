#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>

using namespace std;

vector<string> split(string str, char delimiter) {
    vector<string> internal;
    stringstream ss(str);
    string temp;
    while (getline(ss, temp, delimiter)) {
        internal.push_back(temp);
    }
    return internal;
}

void read_data(string path,
               vector<vector<int> >& node2hyperedge,
               vector<vector<int> >& hyperedge2node,
               vector<unordered_set<int> >& hyperedge2node_set) {
    // first read the data once to reorder nodes into consecutive integers
    ifstream graphFile(path.c_str());
    string line;
    int num_hyperedge = 0, num_node = 0;    
    unordered_map<int, int> node2index;
    while (getline(graphFile, line)) {
        vector<string> nodes = split(line, ',');
        vector<int> tokens;
        unordered_set<int> tokens_set;
        for (int i = 0; i < nodes.size(); i++) {
            int node_i = stoi(nodes[i]);
            if (node2index.find(node_i) == node2index.end()) {
                node2index[node_i] = num_node;
                num_node++;
            }
            int node_i_new = node2index[node_i];
            tokens.push_back(node_i_new);
            tokens_set.insert(node_i_new);
            while (node2hyperedge.size() <= node_i_new) {
                node2hyperedge.push_back(vector<int>());
            }
            node2hyperedge[node_i_new].push_back(num_hyperedge);
        }
		hyperedge2node.push_back(tokens);
		hyperedge2node_set.push_back(tokens_set);
        num_hyperedge++;
    }
}
