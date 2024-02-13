#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include "read_data.cpp"
#include "tools.cpp"


using namespace std;

int main(int argc, char *argv[])
{
	clock_t start;

	// UPDATE: use mt19937 random number generator
	random_device rd;
	mt19937 gen(rd());

	// INPUT: (1) original hypergraph (2) number of Lvels
	// execute run_nav.sh
	//
	// OUTPUT: generated hypergraph

	string dataname = argv[1];
	// int L = stoi(argv[2]);

	string graphFile = "../dataset/" + dataname + "_graph.txt";
	string genFile = "../dataset/" + dataname + "_hypercl.txt";

	// Read data
	start = clock();
	vector< vector<int> > node2hyperedge;
	vector< vector<int> > hyperedge2node;
	vector< unordered_set<int> > hyperedge2node_set;
	read_data(graphFile, node2hyperedge, hyperedge2node, hyperedge2node_set);

	int V = (int)node2hyperedge.size();
	int E = (int)hyperedge2node.size();
	cout << "# of nodes: " << V << endl;
	cout << "# of hyperedges: " << E << endl;
	cout << "Reading data done: " 
	     << (double)(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;
	cout << "--------------------------------------------------------------------" << endl << endl;

	// Preprocessing
	start = clock();

	/* node degree & hyperedge size */
	vector<int> Vdeg;
	vector<int> Esize; 

	for (int i = 0; i < V; i++) Vdeg.push_back((int)node2hyperedge[i].size());
	for (int i = 0; i < E; i++) Esize.push_back((int)hyperedge2node[i].size());

	// random_shuffle(Vdeg.begin(), Vdeg.end());
	// random_shuffle(Esize.begin(), Esize.end());
	shuffle(Esize.begin(), Esize.end(), gen);

	int max_Esize = *max_element(Esize.begin(), Esize.end());
	cout << "Max hyperedge size: " << max_Esize << endl;
	
	discrete_distribution<> d(Vdeg.begin(), Vdeg.end());

	/* sample hyperedges */
	// for each hyperedge, we sample nodes with probabilities proportional to their degrees
	vector< vector<int> > E2V;
	for (int i = 0; i < E; i++){
		vector<int> hyperedge;
		for (int j = 0; j < Esize[i]; j++){
			// sample a node with probabilities proportional to their degrees, i.e., Vdeg
			int idx = -1;
			while (idx == -1){
				idx = d(gen);
				if (hyperedge2node_set[i].find(idx) != hyperedge2node_set[i].end()) idx = -1;
			}			
			hyperedge.push_back(idx);
		}
		E2V.push_back(hyperedge);
	}

	// File output
	start = clock();
	ofstream outFile(genFile.c_str());

	for (int i = 0; i < E; i++){
		if (E2V[i].size() > 0){
			string line = "";
			for (const int &v: E2V[i]) line += to_string(v) + ",";
			line = line.substr(0, line.size() - 1);
			outFile << line << endl;
		}
	}

	outFile.close();

	cout << "File output done: " 
	     << (double)(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;
	cout << "--------------------------------------------------------------------" << endl << endl;



	return 0;
}
