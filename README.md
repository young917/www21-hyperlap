# How Do Hyperedges Overlap in Real-World Hypergraphs? - Patterns, Measures, and Generators

Source code for (1) observation of overlapping patterns of real-world hypergraphs and (2) our graph generative model HyperLAP, described in the paper: How Do Hyperedges Overlap in Real-World Hypergraphs? - Patterns, Measures, and Generators, Geon Lee*, Minyoung Choe*, and Kijung Shin, WWW 2021.

(1) Observation of **overlapping patterns** of real-world hypergraphs
* *Egonet Level*: Substantial overlaps of hyperedges in the egonet of each node in real-world hypergraphs.
* *Pair/Triple of Nodes Level*: Heavy tailed distribution of the number of hyperedges overlapping at each pair or triple of nodes in real-world hypergraphs.
* *Hyperedge Level*: Tendency of hyperedges to contain nodes that are structurally similar (i.e., nodes where more hyperedges overlap) in real-world hypergraphs. 

(2) Our graph generative model **HyperLap**
* *HyperLap*: A realistic hypergraph generative model, which is an extension of HyperCL to multiple levels.
* *HyperLap+*: An automatic parameter tuning algorithm for HyperLap.

## Datasets

In the papaer, we used datasets after removing duplicated hyperedges and size-1 hyperedges. We preprocessed thirteen datasets collected We preprocessed datasets collected by [Austin R. Benson][https://www.cs.cornell.edu/~arb/]. The datasets used in the paper are available in the "dataset" folder.


## Reproduction of Observations

You can run 5 observations with thirteen datasets at the same time.
The outputs will be saved in observation/results/[*data name*] directory.

```
./run.sh
```

The default input file is dataset/[*data name*]_graph.txt

If you want to change inputs (dataset/[*data name*]_[*sth*].txt), change like this:

for example in line 11 of run.sh ,

```
./run_density $data --file_type sth
```

## Generating Hypergraphs

You can run HyperLap with uniform weights by

```
./run_nav.sh
```

You can run HyperLap+ by

```
./run_adv.sh
```

## Contents

```
datasets
    |__ email-Eu-full_graph.txt
    
observation
    |__ egonet_density.cpp
    |__ egonet_overlapness.cpp
    |__ pair_degree.cpp
    |__ triple_degree.cpp
    |__ hyperedge_homogeneity.cpp
    |__ run.sh
    |__ results
        |__ email-Eu-full
                |__ egonet_density_graph.txt			// result of egonet_density.cpp
                |__ egonet_overlapness_graph.txt		// result of egonet_overlapness.cpp
                |__ pair_degree_graph.txt				// result of pair_degree.cpp
                |__ triple_degree_graph.txt			// result of triple_degree.cpp
                |__ hyperedge_homogeneity_graph.txt	// result of hyperedge_homogeneity.cpp
        |__ powerlaw_test.py
        |__ run_powerlaw_test.sh
        |__ plot_results.py

generator
    |__ main_adv.cpp	// HyperLap+
    |__ main_nav.cpp	// HyperLap (uniform weights)
    |__ read_data.cpp	
    |__ tools.cpp
    |__ run_adv.sh	// Run HyperLap+
    |__ run_nav.sh	// Run HyperLap
```
