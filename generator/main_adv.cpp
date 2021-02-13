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
#define P 20 // hyperedge flow rate

using namespace std;

int main(int argc, char *argv[])
{
	clock_t start, runtime;

	// INPUT: original hypergraph
	// OUTPUT: generated hypergraph
	string dataname = argv[1];

	string graphFile = "../dataset/" + dataname + "_graph.txt";
	string genFile = "../dataset/" + dataname + "_gen.txt";

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
	cout << "---------------------------------------------------------------------" << endl << endl;

	// Preprocessing
	start = clock();

	/* node degree & hyperedge size */
	vector<int> Vdeg;
	vector<int> Esize;

	for (int i = 0; i < V; i++) Vdeg.push_back((int)node2hyperedge[i].size());
	for (int i = 0; i < E; i++) Esize.push_back((int)hyperedge2node[i].size());

	random_shuffle(Vdeg.begin(), Vdeg.end());
	random_shuffle(Esize.begin(), Esize.end());

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
	cout << "---------------------------------------------------------------------" << endl << endl;

	runtime = clock();
	// Hypergraph generation (0): Preprocessing
	start = clock();
	int max_num_level = (int)log2(V);
	vector<int> level2Ecnt;
	vector< vector<int> > level2group2V;
	vector< vector< vector<int> > > level2group2E;
	vector< vector< vector<int> > > level2group2Vlist;
	vector< vector< vector<bool> > > level2group2Evisit;
	vector<int> best_level2Ecnt;
	vector< vector< vector<int> > > best_level2group2E;
	vector< vector< vector<bool> > > best_level2group2Evisit;

	level2Ecnt.resize(max_num_level, 0);
	level2group2V.resize(max_num_level);
	level2group2E.resize(max_num_level);
	level2group2Vlist.resize(max_num_level);
	level2group2Evisit.resize(max_num_level);
	best_level2Ecnt.resize(max_num_level, 0);
	best_level2group2E.resize(max_num_level);
	best_level2group2Evisit.resize(max_num_level);

	for (int level = 0; level < max_num_level; level++){
		int num_group = (int)pow(2, level);

		level2group2V[level].resize(num_group, 0);
		level2group2E[level].resize(num_group);
		level2group2Vlist[level].resize(num_group);
		level2group2Evisit[level].resize(num_group);
		best_level2group2E[level].resize(num_group);
		best_level2group2Evisit[level].resize(num_group);

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
	cout << "---------------------------------------------------------------------" << endl << endl;

	// Hypergraph generation (1): Initialization
	start = clock();
	double cur_dist = 1e9;
	unordered_set<int> N;
	vector<double> homogeneity;
	vector< vector<int> > V2E;
	vector< vector<int> > E2V;
	vector< pair<int, int> > E2levelgroup;

	V2E.resize(V); 
	E2V.resize(E);

	/* generate hyperedges */
	int level = 0, group = 0;

	for (int e = 0; e < E; e++){
		int e_size = Esize[e];

		N.clear();
		while (N.size() < e_size){
			int idx = rand() % level2group2Vlist[level][group].size();
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
		E2levelgroup.push_back({level, group});
	}
	
	/* compute hyperedge homogeneity */
	get_homogeneity(homogeneity, V2E, E2V);
	homogeneity_max = *max_element(homogeneity.begin(), homogeneity.end());
	homogeneity_dist = get_D_double(E_homogeneity, homogeneity);
	cur_dist = homogeneity_dist;

	cout << "HyperCL\t"
	     << "Homogeneity Max:\t" << homogeneity_max << "\tHomogeneity D-stat:\t" << homogeneity_dist << endl;

	cout << "Hypergraph generation (1): Initialization done: " 
	     << (double)(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;
	cout << "---------------------------------------------------------------------" << endl << endl;

	// Hypergraph generation (2): Multilevel
	start = clock();
	int MAX_TRIAL = E * 5;
	vector< vector<int> > pre_V2E, best_V2E;
	vector< vector<int> > pre_E2V, best_E2V;
	vector<int> updE;

	pre_V2E.resize(V); best_V2E.resize(V);
	pre_E2V.resize(E); best_E2V.resize(E);

	best_V2E = V2E;
	best_E2V = E2V;
	best_level2Ecnt = level2Ecnt;
	best_level2group2E = level2group2E;
	best_level2group2Evisit = level2group2Evisit;

	for (int level = 1; level < max_num_level; level++){
		int pre_level = level - 1;
		int num_group = (int)pow(2, level);
		bool updated = false;
		int upd_p = -1;

		V2E = best_V2E;
		E2V = best_E2V;
		level2Ecnt = best_level2Ecnt;
		level2group2E = best_level2group2E;
		level2group2Evisit = best_level2group2Evisit;

		for (int p = 0; p < P; p++){
			pair<int, int> _p = get_index(level2Ecnt[pre_level], P, p);
			int upd_Ecnt = _p.second - _p.first;
			int trial = 0;
			updE.clear();

			pre_V2E = V2E;
			pre_E2V = E2V;
			
			/* update hyperedges (level l -> level l+1) */
			while (level2Ecnt[level] < upd_Ecnt * (p + 1) and trial++ < MAX_TRIAL){
				int pre_group = rand() % (int)pow(2, pre_level);
				if (level2group2E[pre_level][pre_group].size() == 0) continue;

				int idx = rand() % level2group2E[pre_level][pre_group].size();
				if (level2group2Evisit[pre_level][pre_group][idx]) continue;

				int e = level2group2E[pre_level][pre_group][idx];
				int e_size = Esize[e];

				int group = rand() % (int)pow(2, level);
				if (e_size > level2group2V[level][group]) continue;

				N.clear();
				while (N.size() < e_size){
					int _idx = rand() % level2group2Vlist[level][group].size();
					int v = level2group2Vlist[level][group][_idx];
					N.insert(v);
				}

				updE.push_back(e);
				E2V[e].clear();
				for (const int &v: N) E2V[e].push_back(v);
				E2levelgroup[e] = {level, group};

				level2group2E[level][group].push_back(e);
				level2group2Evisit[level][group].push_back(false);
				level2group2Evisit[pre_level][pre_group][idx] = true;
				level2Ecnt[level]++;
			}

			/* update V2E and E2V */
			for (const int &e: updE){
				for (const int &v: pre_E2V[e])
					V2E[v].erase(remove(V2E[v].begin(), V2E[v].end(), e), V2E[v].end());
				for (const int &v: E2V[e])
					V2E[v].push_back(e);
			}
	
			/* compute hyperedge homogeneity */
			homogeneity.clear();
			get_homogeneity(homogeneity, V2E, E2V);

			homogeneity_max = *max_element(homogeneity.begin(), homogeneity.end());
			homogeneity_dist = get_D_double(E_homogeneity, homogeneity);
	
			if (homogeneity_dist < cur_dist){
				updated = true; upd_p = p;
				cur_dist = homogeneity_dist;
				best_V2E = V2E;
				best_E2V = E2V;
				best_level2Ecnt = level2Ecnt;
				best_level2group2E = level2group2E;
				best_level2group2Evisit = level2group2Evisit;
			}

			cout << "Level:\t" << level << "\t(p = " << p << ")\t"
			     << "Homogeneity Max:\t" << homogeneity_max << "\tHomogeneity D-stat:\t" << homogeneity_dist << endl;
		}
		cout << "Level:\t" << level << "\toptimal p:\t" << upd_p << endl;
		if (!updated) break;
	}

	cout << "Final hyperedge homogeneity:\t" << cur_dist << endl;
	cout << endl;
	vector<int> EperLevel(max_num_level, 0);
	for (int i = 0; i < E; i++) EperLevel[E2levelgroup[i].first]++;
	for (int i = 0; i < max_num_level; i++) cout << "Level " << i << ":\t" << EperLevel[i] << endl;

	cout << "Hypergraph generation (2): Multilevel done: " 
	     << (double)(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;
	cout << "---------------------------------------------------------------------" << endl << endl;

	cout << "FINAL RUNTIME: " << (double)(clock() - runtime) / CLOCKS_PER_SEC << " sec" << endl;

	// Node reindexing
	start = clock();
	int Vindex = 0;
	unordered_map<int, int> node2index;
	unordered_set<int> node_visit;

	for (int i = 0; i < E; i++){
		if (best_E2V[i].size() > 0){
			for (const int &v: best_E2V[i]){
				bool v_in = (node_visit.find(v) != node_visit.end());
				if (!v_in) node2index[v] = Vindex++;
				node_visit.insert(v);
			}
		}
	}

	cout << "Node reindexing done: " 
	     << (double)(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;
	cout << "---------------------------------------------------------------------" << endl << endl;

	// File output
	start = clock();
	ofstream outFile(genFile.c_str());

	for (int i = 0; i < E; i++){
		if (best_E2V[i].size() > 0){
			string line = "";
			for (const int &v: best_E2V[i]) line += to_string(node2index[v]) + ",";
			line = line.substr(0, line.size()-1);
			outFile << line << endl;
		}
	}

	outFile.close();

	cout << "File output done: " 
	     << (double)(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;
	cout << "---------------------------------------------------------------------" << endl << endl;


	return 0;
}
