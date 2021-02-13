#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

pair<int, int> get_index(int V, int g, int i)
{
	int s = V / g;
	int r = V % g;
	if (i < r) return {(s+1)*i, (s+1)*(i+1)};
	return {(s+1)*r + s*(i-r), (s+1)*r + s*(i+1-r)};
}

void get_degDist(vector<int>& degDist,
		 vector< vector<int> >& node2hyperedge,
		 vector< vector<int> >& hyperedge2node)
{
	int V = (int)node2hyperedge.size();
	int E = (int)hyperedge2node.size();

	for (int i = 0; i < V; i++) degDist.push_back((int)node2hyperedge[i].size());
}

void get_overlapness(vector<double>& overlapness,
		     vector< vector<int> >& node2hyperedge,
		     vector< vector<int> >& hyperedge2node)
{
	int V = (int)node2hyperedge.size();
	int E = (int)hyperedge2node.size();

	unordered_set<int> m;

	for (int v = 0; v < V; v++){
		m.clear();
		double size_sum = 0;
		for (const int &e: node2hyperedge[v]){
			size_sum += (double)hyperedge2node[e].size();
			for (const int &v: hyperedge2node[e]){
				m.insert(v);
			}
		}
		double neigh = (double)m.size();
		overlapness.push_back(size_sum / neigh);
	}
}

void get_homogeneity(vector<double>& locality,
		  vector< vector<int> >& node2hyperedge,
		  vector< vector<int> >& hyperedge2node)
{
	int V = (int)node2hyperedge.size();
	int E = (int)hyperedge2node.size();

	vector< unordered_map<int, int> > V_adj;
	V_adj.resize(V);

	for (int i = 0; i < V; i++){
		for (const int &e: node2hyperedge[i]){
			for (const int &v: hyperedge2node[e]){
				if (v == i) continue;
				V_adj[i][v]++;
			}
		}
	}

	for (int e = 0; e < E; e++){
		double l = 0;
		int e_size = (int)hyperedge2node[e].size();
		for (int i = 0; i < e_size; i++){
			int v_i = hyperedge2node[e][i];
			for (int j = i + 1; j < e_size; j++){
				int v_j = hyperedge2node[e][j];
				l += V_adj[v_i][v_j];
			}
		}
		if (e_size > 1) l /= (e_size * (e_size - 1) / 2);
		locality.push_back(l);
	}
}

double get_bhat(vector<double>& a, vector<double>& b)
{
	int len = (int)a.size(), _max = -1;
	double bc = 0.0;

	unordered_map<int, int> d_a, d_b;
	for (int i = 0; i < len; i++){
		d_a[(int)a[i]]++;
		d_b[(int)b[i]]++;
		_max = max(_max, max((int)a[i], (int)b[i]));
	}

	for (int i = 0; i <= _max; i++){
		bc += pow(((double)d_a[i] / len) * ((double)d_b[i] / len), 0.5);
	}

	return -log(bc);
}

double get_D_int(vector<int>& a, vector<int>& b, bool sorted=false)
{
	int len_a = (int)a.size(), len_b = (int)b.size(), i = 0, j = 0;
	double cum_a = 0, cum_b = 0;
	double max_D = -1;

	if (!sorted){
		sort(a.begin(), a.end());
		sort(b.begin(), b.end());
	}
	
	vector< pair<int, int> > a_dic, b_dic;
	while (i < len_a){
		a_dic.push_back({a[i], 1});
		while (i < len_a-1 && a[i] == a[i+1]){
			a_dic[a_dic.size()-1].second++;
			i++;
		}
		i++;
	}
	while (j < len_b){
		b_dic.push_back({b[j], 1});
		while (j < len_b-1 && b[j] == b[j+1]){
			b_dic[b_dic.size()-1].second++;
			j++;
		}
		j++;
	}

	int sum_a = (int)a_dic.size(), sum_b = (int)b_dic.size();
	i = 0; j = 0;
	while (i < sum_a || j < sum_b){
		if (j == sum_b){
			cum_a += (double)a_dic[i++].second;
		} else if (i == sum_a){
			cum_b += (double)b_dic[j++].second;
		} else if (a_dic[i].first < b_dic[j].first){
			cum_a += (double)a_dic[i++].second;
		} else if (a_dic[i].first > b_dic[j].first){
			cum_b += (double)b_dic[j++].second;
		} else{
			cum_a += (double)a_dic[i++].second;
			cum_b += (double)b_dic[j++].second;
		}
		double D = abs((cum_a/len_a) - (cum_b/len_b));
		max_D = max(max_D, D);
	}

	return max_D;
}

double get_D_double(vector<double>& a, vector<double>& b, bool sorted=false)
{
	int len_a = (int)a.size(), len_b = (int)b.size(), i = 0, j = 0;
	double cum_a = 0, cum_b = 0;
	double max_D = -1;

	if (!sorted){
		sort(a.begin(), a.end());
		sort(b.begin(), b.end());
	}

	vector< pair<double, int> > a_dic, b_dic;
	while (i < len_a){
		a_dic.push_back({a[i], 1});
		while (i < len_a-1 && a[i] == a[i+1]){
			a_dic[a_dic.size()-1].second++;
			i++;
		}
		i++;
	}
	while (j < len_b){
		b_dic.push_back({b[j], 1});
		while (j < len_b-1 && b[j] == b[j+1]){
			b_dic[b_dic.size()-1].second++;
			j++;
		}
		j++;
	}

	int sum_a = (int)a_dic.size(), sum_b = (int)b_dic.size();
	i = 0; j = 0;
	while (i < sum_a || j < sum_b){
		if (j == sum_b){
			cum_a += (double)a_dic[i++].second;
		} else if (i == sum_a){
			cum_b += (double)b_dic[j++].second;
		} else if (a_dic[i].first < b_dic[j].first){
			cum_a += (double)a_dic[i++].second;
		} else if (a_dic[i].first > b_dic[j].first){
			cum_b += (double)b_dic[j++].second;
		} else{
			cum_a += (double)a_dic[i++].second;
			cum_b += (double)b_dic[j++].second;
		}
		double D = abs((cum_a/len_a) - (cum_b/len_b));
		max_D = max(max_D, D);
	}
	
	return max_D;
}
