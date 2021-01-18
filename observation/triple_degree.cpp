#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <random>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "read_data.cpp"
#include "tools.cpp"

using namespace std;

/*
INPUT  
        the name of dataset (necessary)
        --graph_type can be Real / HyperCL / HyperPA / HyperFF / HyperLAP / HyperLAP+.
        --binsize is optional, default size of bin is the number of hyperedges + 1.
OUTOUT
        Degree distribution of triples of nodes.
        In the output file "./results/" + dataname + "/triple_degree_" + graphType + ".txt",
        store '# overlapping hyperedges'    :   '# node triples' table.
        Each row    x                       :           y               means there exists y node triples that share x number of hyperedges.
*/

int main (int argc, char *argv[]){
    clock_t start;

    string dataname = argv[1];
    int bin_size = 0;
    string graphType = "";

    for(int i=2; i<argc ; i++){
        string input = argv[i];
        if(input.compare("--graph_type") == 0) graphType = argv[++i];
        else if(input.compare("--binsize") == 0) bin_size = atoi(argv[++i]);
    }

    string dirpath = "../dataset/";
    string graphFile = dirpath + dataname + "_" + graphType + ".txt";
    cout << "Load " << graphFile << endl;

    // Read data ----------------------------------------------------------------------------------------
	start = clock();
    vector< vector <int> > hyperedge2node;
    vector< vector <int> > node2hyperedge;

    read_data(graphFile, node2hyperedge, hyperedge2node);
    
    int V = node2hyperedge.size();
    int E = hyperedge2node.size();

	cout << "# of nodes: " << V << '\n';
	cout << "# of hyperedges: " << E << '\n';
    cout << "Reading data done: "
		<< (double)(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;
	cout << "-----------------------------------------------------------" << endl << endl;

    // Get adjacency list -------------------------------------------------------------------------------
    start = clock();
    vector< vector< pair<int, int> > > node_adj; // store < neighbor node, how many hyperedges share each other >
    vector< unordered_map<int, int> > node_inter;
    
    project(hyperedge2node, node2hyperedge, node_adj, node_inter);

	cout << "Find node's neighbors: "
		<< (double)(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;
	cout << "-----------------------------------------------------------" << endl << endl;

    
    // Fill bins according to degree of node triple -------------------------------------------------------
    if(bin_size == 0) bin_size = E+1;
    start = clock();
    vector< int > bin(bin_size, 0);

    for(int va=0 ; va < V ; va++){
        int deg_a = (int)node_adj[va].size();

        for(int i=0 ; i < deg_a ; i++){
            int vb = node_adj[va][i].first;
            int deg_b = (int)node_adj[vb].size();

            vector<int> intersection;
            for(auto &h : node2hyperedge[va]){
                if(find(node2hyperedge[vb].begin(), node2hyperedge[vb].end(), h) != node2hyperedge[vb].end()){
                    intersection.push_back(h);
                }
            }

            for (int j = i+1; j < deg_a; j++){
                int vc = node_adj[va][j].first;
                    
                int inter_abc = 0;
                if(node_inter[vb][vc] !=0 && va < min(vb,vc)){ // all connected 3 nodes
                    for(auto &h: node2hyperedge[vc]){
                        if(find(intersection.begin(), intersection.end(), h) != intersection.end()){
                            inter_abc++;
                        }
                    }
                }
                else continue; // exclude zero intersection_abc
                int bin_idx = min( bin_size-1, inter_abc);
                bin[bin_idx]++;
            }
        }
    }
    cout << "Find degree of node triple: "
		<< (double)(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;
	cout << "-----------------------------------------------------------" << endl << endl;

    // Write File ----------------------------------------------------------------------------
    start = clock();
    string writeFile = "./results/" + dataname + "/triple_degree_" + graphType + ".txt";
    ofstream resultFile(writeFile.c_str());
    resultFile << "# overlapping hyperedges,# node triples" << endl;
    
    for(int i=1; i<bin_size ; i++){
        resultFile << to_string(i) <<"," << to_string(bin[i]) << endl;
    }
    resultFile.close();

    cout << "Write File: "
		<< (double)(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;
	cout << "-----------------------------------------------------------" << endl << endl;

    return 0;
}    
