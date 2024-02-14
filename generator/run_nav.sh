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
numLevelList=(
	"5"
 	"10"
  	"15"
   	"20"
)

g++ -O3 -std=c++11 main_nav.cpp -o run_nav;
IFS=,
for dataname in $datanameList;
do
	for numLevel in $numLevelList;
	do
		./run_nav $dataname $numLevel;
	done
done
rm run_nav;
