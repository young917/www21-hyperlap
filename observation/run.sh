g++ -g -O3 -std=c++11 egonet_density.cpp -o run_density;
g++ -g -O3 -std=c++11 egonet_overlapness.cpp -o run_overlapness;
g++ -g -O3 -std=c++11 pair_degree.cpp -o run_pair;
g++ -g -O3 -std=c++11 triple_degree.cpp -o run_triple;
g++ -g -O3 -std=c++11 hyperedge_homogeneity.cpp -o run_homogeneity;

dataset=("email-Enron-full" "email-Eu-full" "contact-high" "contact-primary" "NDC-classes-full" "NDC-substances-full" "tags-ubuntu" "tags-math" "threads-ubuntu" "threads-math" "coauth-DBLP-full" "coauth-geology-full" "coauth-history-full")

for data in ${dataset[@]}
do
    ./run_density $data --graph_type graph
    ./run_overlapness $data --graph_type graph
    ./run_pair $data --graph_type graph
    ./run_triple $data --graph_type graph
    ./run_homogeneity $data --graph_type graph    
done
rm run_density
rm run_overlapness
rm run_pair
rm run_triple
rm run_homogeneity