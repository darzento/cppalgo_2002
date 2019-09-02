#include <stdio.h>

#define NVERTEX 4

int Graph[NVERTEX][NVERTEX] = 
{
    { 0, 10, 15, 20 },
	{ 5,  0,  9, 10 },
	{ 6, 13,  0, 12 },
	{ 8,  8,  9,  0 }
};

int count = 0;

bool Promising_Simple(int trial[], int j)
{
	// ��ΰ� �ߺ��Ǹ� �ȵȴ�. 
	for (int i = 0; i < j; i++) {
		if (trial[i] == trial[j]) return false;
	}

	return true;
}

void TSP_Simple(int g[][NVERTEX], int trial[], int best[], int& mincost, int j = 0, int cost = 0) {
	if (Promising_Simple(trial, j)) {
		count++;	// for test
		if (j == NVERTEX - 1) {
			if (cost + g[trial[j]][trial[0]] < mincost) {
				mincost = cost + g[trial[j]][trial[0]];
				for (int i = 0; i < NVERTEX; i++) 
					best[i] = trial[i];
			}
		} else {
			for (int i = 1; i < NVERTEX; i++) {
				trial[j+1] = i;
				TSP_Simple(g, trial, best, mincost, j+1, cost + g[trial[j]][trial[j+1]]);
			}
		}
	}
}

bool Promising_Bound(int g[][NVERTEX], int trial[], int j, int mincost, int cost, int vmin[])
{
	// ��ΰ� �ߺ��Ǹ� �ȵȴ�. 
	for (int i = 0; i < j; i++) {
		if (trial[i] == trial[j]) return false;
	}

	// ���� ������� ���ϱ� 
	int estcost = cost;

	int v, nv;
	bool visited;
	for (v = 0; v < NVERTEX; v++) {
		visited = false;
		for (nv = 0; nv <= j; nv++) {
			if (trial[nv] == v) { 
				visited = true;
				break;
			}
			if (!visited) {
				cost += vmin[nv];
			}
		}
	}
	cost += vmin[0];

	if (cost < mincost) return true;
	else return false;
}

void TSP_Bound(int g[][NVERTEX], int trial[], int best[], int& mincost, int vmin[], int j = 0, int cost = 0) {
	// vmin�� �� ������ ������ ������ ��� �߿��� �ּ� ����� �ǹ��Ѵ�. 
	if (j == 0) {  // ó�� ȣ�� �̸� vmin�� �����Ѵ�. 
		for (int i = 0; i < NVERTEX; i++) {
			int min = 2000000000;
			for (int k = 0; k < NVERTEX; k++) {
				if (g[i][k] < min) min = g[i][k];
				if (g[k][i] < min) min = g[k][i];
			}
			vmin[i] = min;
		}
	}

	if (Promising_Bound(g, trial, j, mincost, cost, vmin)) {
		count++;	// for test
		if (j == NVERTEX - 1) {
			if (cost + g[trial[j]][trial[0]] < mincost) {
				mincost = cost + g[trial[j]][trial[0]];
				for (int i = 0; i < NVERTEX; i++) 
					best[i] = trial[i];
			}
		} else {
			for (int i = 1; i < NVERTEX; i++) {
				trial[j+1] = i;
				TSP_Bound(g, trial, best, mincost, vmin, j+1, cost + g[trial[j]][trial[j+1]]);
			}
		}
	}
}

void main(void)
{
	int mincost = 2000000000;
	int trial[NVERTEX], best[NVERTEX];

	count = 0;
	trial[0] = 0;

	TSP_Simple(Graph, trial, best, mincost);

	printf("Traveling Salesman (Simple version)\n");
	for (int i = 0; i < NVERTEX; i++) {
		printf("%c ", 'A' + best[i]);
	}
	printf("A\n");
	printf("min cost = %d (%d tries)\n", mincost, count);

	count = 0;
	trial[0] = 0;
	mincost = 2000000000;
	int vmin[NVERTEX];

	printf("Traveling Salesman (Bound version)\n");
	TSP_Bound(Graph, trial, best, mincost, vmin);

	for (i = 0; i < NVERTEX; i++) {
		printf("%c ", 'A' + best[i]);
	}
	printf("A\n");
	printf("min cost = %d (%d tries)\n", mincost, count);

}