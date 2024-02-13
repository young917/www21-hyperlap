datanameList=(
	"coauth-DBLP-full"
	"coauth-geology-full"
	"coauth-history-full"
	"contact-high"
	"contact-primary"
	"email-Enron-full"
	"email-Eu-full"
	"NDC-classes-full"
	"NDC-substances-full"
	"tags-math"
	"tags-ubuntu"
	"threads-math"
	"threads-ubuntu"
)

g++ -O3 -std=c++11 main_hypercl.cpp -o run_hypercl
IFS=,
for dataname in "${datanameList[@]}"; do
    ./run_hypercl $dataname
done
rm run_hypercl
