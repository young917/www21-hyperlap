datanameList=(
	"coauth-DBLP-full_graph"
	"coauth-geology-full_graph"
	"coauth-history-full_graph"
	"contact-high_graph"
	"contact-primary_graph"
	"email-Enron-full_graph"
	"email-Eu-full_graph"
	"NDC-classes-full_graph"
	"NDC-substances-full_graph"
	"tags-math_graph"
	"tags-ubuntu_graph"
	"threads-math_graph"
	"threads-ubuntu_graph"
)

g++ -O3 -std=c++11 main_adv.cpp -o run_adv
IFS=,
for dataname in "${datanameList[@]}"; do
    ./run_adv $dataname
done
rm run_adv
