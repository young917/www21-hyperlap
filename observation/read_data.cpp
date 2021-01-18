#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>

using namespace std;

vector<string> split(string str, char delimiter){
	vector<string> internal;
	stringstream ss(str);
	string temp;
	while (getline(ss, temp, delimiter)){
		internal.push_back(temp);
	}
	return internal;
}

void read_data(string path, 
		vector< vector<int> >& node2hyperedge, 
		vector< vector<int> >& hyperedge2node){
	
	ifstream graphFile(path.c_str());
	string line;
	int num_hyperedge = 0;
	bool flag = false;
	while (getline(graphFile, line)){
		vector<string> nodes = split(line, ',');
		vector<int> tokens;
		for (int i = 0; i < nodes.size(); i++){
			tokens.push_back(stoi(nodes[i]));
			while (node2hyperedge.size() <= stoi(nodes[i]))
				node2hyperedge.push_back(vector<int>());
			node2hyperedge[stoi(nodes[i])].push_back(num_hyperedge);
		}
		hyperedge2node.push_back(tokens);
		num_hyperedge++;
	}
}
