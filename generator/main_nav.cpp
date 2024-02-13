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
	int L = stoi(argv[2]);

	string graphFile = "../dataset/" + dataname + "_graph.txt";
	string genFile = "../dataset/" + dataname + "_uniform.txt";

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
	shuffle(Vdeg.begin(), Vdeg.end(), gen);
	shuffle(Esize.begin(), Esize.end(), gen);

	int max_Esize = *max_element(Esize.begin(), Esize.end());
	cout << "Max hyperedge size: " << max_Esize << endl;

	/* hyperedge homogeneity */
	double homogeneity_max, homogeneity_dist;
	vector<double> E_homogeneity;
	get_homogeneity(E_homogeneity, node2hyperedge, hyperedge2node);

	homogeneity_max = *max_element(E_homogeneity.begin(), E_homogeneity.end());
	cout << "Max hyperedge homogeneity: " << homogeneity_max << endl;

	cout << "Preprocessing done: " 
	     << (double)(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;
	cout << "--------------------------------------------------------------------" << endl << endl;

	// Hypergraph generation (0): Preprocessing
	start = clock();
	vector<int> level2Ecnt;
	vector< vector<int> > level2group2V;
	vector< vector< vector<int> > > level2group2E;
	vector< vector< vector<int> > > level2group2Vlist;
	vector< vector< vector<int> > > level2group2Evisit;

	level2Ecnt.resize(L, 0);
	level2group2V.resize(L);
	level2group2E.resize(L);
	level2group2Vlist.resize(L);
	level2group2Evisit.resize(L);

	for (int level = 0; level < L; level++){
		int num_group = (int)pow(2, level);

		level2group2V[level].resize(num_group, 0);
		level2group2E[level].resize(num_group);
		level2group2Vlist[level].resize(num_group);
		level2group2Evisit[level].resize(num_group);

		for (int group = 0; group < num_group; group++){
			pair<int, int> idx = get_index(V, num_group, group);
			int s_idx = idx.first;
			int e_idx = idx.second;
			for (int i = s_idx; i < e_idx; i++){
				level2group2V[level][group]++;
				for (int j = 0; j < Vdeg[i]; j++)
					level2group2Vlist[level][group].push_back(i);
			}
		}
	}

	cout << "Hypergraph generation (0): Preprocessing done: " 
	     << (double)(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;
	cout << "--------------------------------------------------------------------" << endl << endl;

	// Hypergraph generation (1): Hyperedge generation
	start = clock();
	unordered_set<int> N;
	vector< vector<int> > V2E;
	vector< vector<int> > E2V;

	V2E.resize(V);
	E2V.resize(E);

	/* generate hyperedges */
	int level = 0, group = 0;

	for (int e = 0; e < E; e++){
		int e_size = Esize[e];
		
		// level = rand() % L;
		// group = rand() % (int)pow(2, level);

		int pow2level = (int)pow(2, level);

		uniform_int_distribution<> distrib(0, L - 1);
		level = distrib(gen);
		uniform_int_distribution<> distrib2(0, pow2level - 1);
		group = distrib2(gen);		

		while (level2group2V[level][group] < e_size){
			// level = rand() % L;
			// group = rand() % (int)pow(2, level);
			level = distrib(gen);
			group = distrib2(gen);
		}

		N.clear();
		int group_size = level2group2V[level][group];
		while (N.size() < e_size){
			// int idx = rand() % level2group2Vlist[level][group].size();
			uniform_int_distribution<> distrib3(0, group_size - 1);
			int idx = distrib3(gen);
			int v = level2group2Vlist[level][group][idx];
			N.insert(v);
		}

		for (const int &v: N){
			V2E[v].push_back(e);
			E2V[e].push_back(v);
		}

		level2group2E[level][group].push_back(e);
		level2group2Evisit[level][group].push_back(false);
		level2Ecnt[level]++;
	}

	/* compute hyperedge homogeneity */
	vector<double> homogeneity;
	get_homogeneity(homogeneity, V2E, E2V);
	homogeneity_max = *max_element(homogeneity.begin(), homogeneity.end());
	homogeneity_dist = get_D_double(E_homogeneity, homogeneity);

	cout << "UNIFORM\t"
	     << "Homogeneity Max:\t" << homogeneity_max << "\tHomogeneity D-stat:\t" << homogeneity_dist << endl;

	cout << "Hypergraph generation (1: Hyperedge generation done: " 
	     << (double)(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;
	cout << "--------------------------------------------------------------------" << endl << endl;

	// Node reindexing
	start = clock();
	int Vindex = 0;
	unordered_map<int, int> node2index;
	unordered_set<int> node_visit;

	for (int i = 0; i < E; i++){
		if (E2V[i].size() > 0){
			for (const int &v: E2V[i]){
				bool v_in = (node_visit.find(v) != node_visit.end());
				if (!v_in) node2index[v] = Vindex++;
				node_visit.insert(v);
			}
		}	
	}

	cout << "Node reindexing done: " 
	     << (double)(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;
	cout << "--------------------------------------------------------------------" << endl << endl;

	// File output
	start = clock();
	ofstream outFile(genFile.c_str());

	for (int i = 0; i < E; i++){
		if (E2V[i].size() > 0){
			string line = "";
			for (const int &v: E2V[i]) line += to_string(node2index[v]) + ",";
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
