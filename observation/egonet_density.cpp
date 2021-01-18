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

OUTPUT  
        Density of egonets.
        In the output file "./results/" + dataname + "/egonet_density_" + graphType + ".txt",
        store table which index consists of [num_hes,num_nodes,egonet density].
        In detail, each row shows the number of hyperedges in one egonet, the number of nodes in one egonet, and this egonet's density.
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

    // Find egonet overlapness & Write file ---------------------------------------------------------------
    start = clock();
    string writeFile = "./results/" + dataname + "/egonet_density_" + graphType + ".txt";
    
    ofstream resultFile(writeFile.c_str());
    resultFile << "num_hes,num_nodes,egonet density" << endl;
    
    vector<bool> check(V, false);
    for(int v=0 ; v<V ; v++){
        int deg = (int)node2hyperedge[v].size();
        int num_nodes = 0;
        int num_hyperedges = 0;

        fill(check.begin(), check.end(), false);
        check[v] = true;
        num_nodes++;
        
        for(auto &h : node2hyperedge[v]){
            for(auto &node : hyperedge2node[h]){
                if(!check[node]){
                    check[node] = true;
                    num_nodes++;
                }
            }
            num_hyperedges++;
        }
        resultFile << num_hyperedges << "," << num_nodes << "," << to_string((double)num_hyperedges / num_nodes) << endl;
    }
    resultFile.close();

    cout << "Write File: "
		<< (double)(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;
	cout << "-----------------------------------------------------------" << endl << endl;

    return 0;
}    
