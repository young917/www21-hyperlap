#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
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

OUTPUT  
        List of homogeneity of hyperedges.
        In the output file "./results/" + dataname + "/hyperedge_homogeneity_" + graphType + ".txt",
        store homogeneity of one hyperedge in each row.
*/

int main (int argc, char *argv[]){
    clock_t start;

    string dataname = argv[1];
    string graphType = "";

    for(int i=2; i<argc ; i++){
        string input = argv[i];
        if(input.compare("--graph_type") == 0) graphType = argv[++i];
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
    vector< unordered_map<int, int> > node_inter; // store < neighbor node, how many hyperedges share each other >
    project_hashing(node2hyperedge, hyperedge2node, node_inter);

	cout << "Find hyperedge's neighbors: "
		<< (double)(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;
	cout << "-----------------------------------------------------------" << endl << endl;

    // Output homogeneity of hyperedges -----------------------------------------------------------------
    start = clock();
    string outputdir = "./results/" + dataname;
    mkdir(outputdir.c_str(), 0776);
    string writeFile = outputdir + "/hyperedge_homogeneity_" + graphType + ".txt";
    
    ofstream resultFile(writeFile.c_str());
    resultFile << "homogeneity" << endl;
    
    for(int h=0 ; h<E ; h++){
        int hsize = (int)hyperedge2node[h].size();
        
        int sum_common_hyperedges = 0;
        int total_comb = 0;
        for(int i=0; i<hsize; i++){
            for(int j=i+1 ; j<hsize ; j++){
                int node_i = hyperedge2node[h][i];
                int node_j = hyperedge2node[h][j];
                sum_common_hyperedges += node_inter[node_i][node_j];
                total_comb++;
            }
        }
        double homogeneity = 0;
        if(total_comb != 0) homogeneity = (double)sum_common_hyperedges / total_comb; 
        resultFile << to_string(homogeneity) << endl;
    }
    resultFile.close();

    cout << "Write File: "
		<< (double)(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;
	cout << "-----------------------------------------------------------" << endl << endl;

    return 0;
}    
