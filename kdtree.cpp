#include <bits/stdc++.h>

using namespace std;
const int featurenum = 17;
const int featuresize = 100;
const double inf = 1e10;
const int K = 8;
typedef vector<double> Feature;
typedef vector<vector<double>> Featurelist;
Featurelist listfeature;
const int Troot = 0;
vector<int> child[featuresize + 10];
vector<double> nowfeature;
vector<int> res;
const int Lmax = 50;
const int kpoint = 10;
int nodepool = featuresize + 1;
int vst[1005];
int isLeaf[1005];
int L;
inline double getDistance(const Feature& f1, const Feature& f2)
{
	assert(f1.size() == featurenum && f2.size() == featurenum);
	double ret = 0;
	for (int i = 0; i < featurenum; i++)
	{
		ret += (f1[i] - f2[i]) * (f1[i] - f2[i]);
	}
	return ret;
}


struct featureNode
{
	//return true;
	featureNode(int _id) :id(_id) {};
	int id;
	bool operator<(const featureNode& ano) const
	{
		if (getDistance(listfeature[id], nowfeature) < getDistance(listfeature[ano.id], nowfeature))
		{
			return false;
		}
		return true;
	}
};
priority_queue<featureNode, vector<featureNode>> Apointq;
priority_queue<featureNode, vector<featureNode>> Fpointq;

void generateFeature()
{
	vector<double> tmpFeature;
	for (int i = 0; i < featurenum; i++)
	{
		double tval = (rand() % 1000000) / 1000.0;
		tmpFeature.push_back(tval);
	}
	listfeature.push_back(std::move(tmpFeature));
}

void randomFeature()
{
	nowfeature.clear();
	for (int i = 0; i < featurenum; i++)
	{
		double tval = (rand() % 1000000) / 1000.0;
		nowfeature.push_back(tval);
	}
}
void addDepth(int u)
{
	if (child[u].size() <= K)
	{
		isLeaf[u] = true;
		return;
	}
	for (int i = 0; i < K; i++)
	{
		listfeature.push_back(listfeature[child[u][i]]);
		int tmp = child[u][i];
		child[u][i] = nodepool++;
		child[nodepool - 1].push_back(tmp);
	}
	for (int i = K; i < child[u].size(); i++)
	{
		double mindis = getDistance(listfeature[child[u][0]], listfeature[child[u][i]]);
		int minid = 0;
		for (int j = 1; j < K; j++)
		{
			double tdis = getDistance(listfeature[child[u][i]], listfeature[child[u][j]]);
			if (tdis < mindis)
			{
				mindis = tdis;
				minid = j;
			}
		}
		child[child[u][minid]].push_back(child[u][i]);
	}
	child[u].resize(K);
	for (int i = 0; i < K; i++)
	{
		addDepth(child[u][i]);
	}
}
void buildKDTree()
{
	for (int i = 1; i <= featuresize; i++)
	{
		child[0].push_back(i);
	}
	addDepth(0);
}

void traverseTree(featureNode u)
{
	int ui = u.id;
	if (isLeaf[ui])
	{
		L += child[ui].size();
		for (int i = 0; i < child[ui].size(); i++)
		{
			Fpointq.emplace(child[ui][i]);
		}
		return;
	}
	int minid = 0;
	double mindis = getDistance(nowfeature, listfeature[child[ui][0]]);
	for (int i = 1; i < child[ui].size(); i++)
	{
		double tdis = getDistance(nowfeature, listfeature[child[ui][i]]);
		if (tdis < mindis)
		{
			minid = i;
			mindis = tdis;
		}
	}
	for (int i = 0; i < child[ui].size(); i++)
	{
		if (i != minid)
		{
			int v = child[ui][i];
			if (!vst[v])
			{
				Apointq.emplace(v);
				vst[v] = true;
			}
		}
	}
	featureNode tmp(child[ui][minid]);
	traverseTree(move(tmp));
}

void search()
{
	L = 0;
	while (!Apointq.empty())
		Apointq.pop();
	while (!Fpointq.empty())
		Fpointq.pop();
	featureNode tmp(0);
	vst[0] = true;
	traverseTree(move(tmp));
	while (L <= Lmax && !Apointq.empty())
	{
		featureNode fn = Apointq.top();
		Apointq.pop();
		traverseTree(fn);
	}
	for (int i = 0; i < kpoint && !Fpointq.empty(); i++)
	{
		featureNode tmp = Fpointq.top();
		Fpointq.pop();
		res.push_back(tmp.id);
	}
}
void init()
{
	vector<double> allzero;
	for (int i = 0; i < featurenum; i++)
	{
		allzero.push_back(0);
	}
	listfeature.push_back(move(allzero));
	for (int i = 0; i < featuresize; i++)
	{
		generateFeature();
	}
	memset(vst, false, sizeof(vst));
	memset(isLeaf, false, sizeof(isLeaf));
}

int main()
{
	srand(42);
	init();
	buildKDTree();
	randomFeature();
	search();
	for (int i = 0; i < kpoint; i++)
	{
		printf("%d ", res[i]);
	}
	printf("\n");
	for (int i = 0; i < featuresize; i++)
	{
		if (i % 10 == 0)
			printf("\n");
		printf("%4.2f  ", getDistance(listfeature[i + 1], nowfeature));
	}
}