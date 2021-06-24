#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Edge {
public:
    int src;
    int des;
    int w;
};

class Graph {
public:
    int V,E;
    Edge * edges;
};

void BF(Graph G,int s) {
    int n = G.V;
    int m = G.E;
    int A[n];
    int P[n];
    for (int i = 0; i< n; i++) {
        P[i]= -1;
    }
    for (int i = 0; i < n; i++) {
        if (i == s) {
            A[i] = 0;
        } else {
            A[i] = INT8_MAX;
        }
    }
    // n-1 iterations to get the shortest path
    for (int i = 1; i < n; i++) {
        int k = 0;
        for (int j = 0; j < m; j++) {
            if (A[G.edges[j].src] != INT8_MAX) {
                if ( A[G.edges[j].des] > A[G.edges[j].src] + G.edges[j].w){
                    k = 1;
                    A[G.edges[j].des] = A[G.edges[j].src] + G.edges[j].w;
                    P[G.edges[j].des] = G.edges[j].src;
                }
            }
        }
        if (!k) {
            break;
        }
    }
    // to detect negative cycle
    for (int j = 0; j < m; j++) {
        if (A[G.edges[j].src] != INT8_MAX) {
            if ( A[G.edges[j].des] > A[G.edges[j].src] + G.edges[j].w){
                cout << "ERROR: NEGATIVE WEIGHT CYCLE" << endl;
                vector<int> trace;
                int current = G.edges[j].des;
                while (find(trace.begin(),trace.end(),current) == trace.end()) {
                    trace.push_back(current);
                    current = P[current];
                }
                for(int i = 0,k = 0; i < trace.size();i++) {
                    if (trace[i] == current) {
                        k = 1;
                    }
                    if (k == 1) {
                        cout << trace[i] << " ";
                    }
                }
                cout << current << endl;
                return;
            }
        }
    }
    for (int i = 0; i < G.V; i++) {
        if (A[i] != INT8_MAX){
            cout << A[i] << " ";
        } else {
            cout << "inf ";
        }
    }
}


int main() {
    int n,m,s;
    cin >> n;
    cin >> m;
    cin >> s;
    Graph G;
    G.V = n;
    G.E = m;
    G.edges = new Edge[m];
    for (int i = 0; i < m; i++) {
        int src,des,w;
        cin >> src;
        cin >> des;
        cin >> w;
        G.edges[i].src = src;
        G.edges[i].des = des;
        G.edges[i].w = w;
    }
    
    BF(G, s);
}
