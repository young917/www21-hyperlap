#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <vector>

using namespace std;

void project_hashing(vector< vector<int> > &node2hyperedge,
                        vector< vector<int> > &hyperedge2node,
                        vector< unordered_map<int, int> > &node_inter){
	// input node2hyperedge, hyperedge2 node
	// output clique expansion result into the unordered map datastructure
    
    int V = (int)node2hyperedge.size();
	node_inter.resize(V);

	for (int i = 0; i < V; i++){
		for (const int &e: node2hyperedge[i]){
			for (const int &v: hyperedge2node[e]){
				if (v == i) continue;
				node_inter[i][v]++;
			}
		}
	}
}

void project(vector< vector<int> > &hyperedge2node,
            vector< vector<int> > &node2hyperedge,
            vector< vector< pair<int, int> > > &node_adj, 
            vector< unordered_map<int, int> > &node_inter){
	// input node2hyperedge, hyperedge2 node
	// output clique expansion result into the unordered map and adjacency list

	int V = (int)node2hyperedge.size();
    node_adj.resize(V);
    node_inter.resize(V);
	vector<long long> upd_time(V, -1LL);
	
	for (int node_a = 0; node_a < V; node_a++){
		for (const int &hyperedge: node2hyperedge[node_a]){
			for (const int &node_b: hyperedge2node[hyperedge]){
				if (node_a == node_b) continue;
				if ((upd_time[node_b] >> 31) ^ node_a){
					upd_time[node_b] = ((long long)node_a << 31)
							      + (long long)node_adj[node_a].size();
					node_adj[node_a].push_back({node_b, 0});
				}
				node_adj[node_a][(int)(upd_time[node_b] & 0x7FFFFFFFLL)].second++;
			}
		}
	}
	for (int node_a = 0; node_a < V; node_a++){
        node_inter[node_a].rehash((int)node_adj[node_a].size());
        for (int i = 0; i < node_adj[node_a].size(); i++){
            int node_b = node_adj[node_a][i].first;
            int C_ab = node_adj[node_a][i].second;
            node_inter[node_a].insert({node_b, C_ab});
        }
    }
}